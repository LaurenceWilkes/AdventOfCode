using Graphs, SimpleWeightedGraphs, LongestPaths

function getarray(filename)
    filepath = joinpath(dirname(@__FILE__), "$(filename).txt")
    a = []
    open(filepath) do file
        for ln in eachline(file)
            push!(a,[])
            for char in ln
                push!(a[end],char)
            end
        end
    end
    return mapreduce(permutedims,vcat,a)
end

function mapdict(map)
    Q = Dict()
    for ij in CartesianIndices(map)
        Q[ij] = map[ij]
    end
    startpt = CartesianIndex(1,1)
    stoppt = CartesianIndex(1,1)
    for (j,col) in enumerate(eachcol(map))
        col[1] == '.' && (startpt = CartesianIndex(1,j))
        col[end] == '.' && (stoppt = CartesianIndex(size(map,1),j))
    end
    return Q,startpt,stoppt
end

# function buildgraph(map)
#     startpt, stoppt = startstop(map)
#     g = SimpleWeightedDiGraph(1)
#     Q = Dict(startpt=>1)
#     curpt = startpt
#     Dirs = [CartesianIndex(-1,0),CartesianIndex(0,1),CartesianIndex(1,0),CartesianIndex(0,-1)]
#     Dirpos = ['^','>','V','<']
#     fromdir = 1
#     Todo = []
#     npflag = false
#     backwardflag = false
#     forwardflag = false
#     curlen = 1
#     while !isempty(Todo)
#         npflag && ((curpt, fromdir) = popfirst!(Todo); npflag = false)
#         if haskey(Q,curpt)
#             includeedge!(g,Q,curpt,prevvert,curlen,backwardflag,forwardflag)
#             npflag = true
#             continue
#         end
#         Iposs = []
#         for (i,d) in enumerate(Dirs)
#             i == fromdir && continue
#             futpt = curpt + d
#             dp = get(map,futpt,nothing)
#             if dp == '.'
#                 push!(Iposs,(i,1))
#             elseif dp == Dirpos[i]
#                 push!(Iposs,(i,2))
#             elseif dp == Dirpos[mod1(i+2,4)]
#                 push!(Iposs,(i,3))
#             end
#         end
#         if length(Iposs) != 1
#             add_vertex!(g)
#             Q[curpt] = nv(g)
#             includeedge!(g,Q,curpt,prevvert,curlen,backwardflag,forwardflag)
#             backwardflag = false
#             forwardflag = false
#             for i in Iposs
#                 push!(Todo,(curpt + Dirs[i],mod1(i+2,4)))
#                 curlen = 1
#             end
#             npflag = true
#         else
#             if dp == '.'
#                 push!(Iposs,i)
#             elseif dp == Dirpos[i]
#                 forwardflag = true
#                 push!(Iposs,i)
#             elseif dp == Dirpos[mod1(i+2,4)]
#                 backwardflag = true
#                 push!(Iposs,i)
#             end
#         end
#     end

# end

function includeedge!(g,prevvert,newvert,curlen,backwardflag,forwardflag)
    # add_edge!(g,newvert,prevvert,-curlen)
    # add_edge!(g,prevvert,newvert,-curlen)
    if forwardflag
        if backwardflag
        else
            add_edge!(g,prevvert,newvert,curlen)
        end
    else
        if backwardflag
            add_edge!(g,newvert,prevvert,curlen)
        else
            add_edge!(g,newvert,prevvert,curlen)
            add_edge!(g,prevvert,newvert,curlen)
        end
    end
end

function buildgraph(startpt,map)
    g = SimpleWeightedDiGraph(1)
    Q = Dict(startpt=>1)
    Dirs = [CartesianIndex(-1,0),CartesianIndex(0,1),CartesianIndex(1,0),CartesianIndex(0,-1)]
    Todo = [(startpt,1,1)]
    while !isempty(Todo)
        (curpt, fromdir, prevvert) = popfirst!(Todo)
        pathlen, curpt, Iposs, backwardflag, forwardflag = followpath!(map,curpt,fromdir)
        if haskey(Q,curpt)
            newvert = Q[curpt]
            includeedge!(g,prevvert,newvert,pathlen,backwardflag,forwardflag)
        elseif length(Iposs) == 0
            add_vertex!(g)
            Q[curpt] = nv(g)
            newvert = Q[curpt]
            includeedge!(g,prevvert,newvert,pathlen,backwardflag,forwardflag)
        else
            add_vertex!(g)
            Q[curpt] = nv(g)
            newvert = Q[curpt]
            includeedge!(g,prevvert,newvert,pathlen,backwardflag,forwardflag)
            for (i,fbf) in Iposs
                push!(Todo,(curpt+Dirs[i],mod1(i+2,4),newvert))
            end
        end
    end
    return g, Q
end

function followpath!(map,start,fromdir)
    Dirs = [CartesianIndex(-1,0),CartesianIndex(0,1),CartesianIndex(1,0),CartesianIndex(0,-1)]
    Dirpos = ['^','>','v','<']
    curpt = start
    forwardflag, backwardflag = false, false
    map[curpt] == Dirpos[mod1(fromdir+2,4)] && (forwardflag = true)
    map[curpt] == Dirpos[fromdir] && (backwardflag = true)
    pathlen = 1
    while true
        Iposs = []
        for (i,d) in enumerate(Dirs)
            i == fromdir && continue
            futpt = curpt + d
            dp = get(map,futpt,nothing)
            if dp == '.'
                push!(Iposs,(i,1))
            elseif dp == Dirpos[i]
                push!(Iposs,(i,2))
            elseif dp == Dirpos[mod1(i+2,4)]
                push!(Iposs,(i,3))
            end
        end
        if length(Iposs) != 1
            return pathlen, curpt, Iposs, backwardflag, forwardflag
        else
            i,fbf = Iposs[1]
            curpt = curpt + Dirs[i]
            fbf == 2 && (forwardflag = true)
            fbf == 3 && (backwardflag = true)
            fromdir = mod1(i+2,4)
            pathlen += 1
        end
    end
end

function longest_path(g, startvert, stopvert)
    visited = zeros(Bool,nv(g))
    maxlen = 0

    function dfs(pos,len)
        pos == stopvert && return len
        for n in neighbors(g,pos)
            visited[n] && continue
            w = get_weight(g,pos,n)
            visited[n] = true
            t = dfs(n,len+w)
            t > maxlen && (maxlen = t)
            visited[n] = false
        end
        return 0
    end
    dfs(startvert,0)
    return maxlen
end

function longestpath(filename)
    map,startpt,stoppt = mapdict(getarray(filename))
    g, Q = buildgraph(startpt,map)
    g = SimpleWeightedGraph(g)
    # gplot(g)
    # result = dijkstra_shortest_paths(g, Q[startpt])
    # return result.dists[Q[stoppt]]
    longest_path(g,Q[startpt],Q[stoppt])
    # println(g)
end

@time longestpath("input")