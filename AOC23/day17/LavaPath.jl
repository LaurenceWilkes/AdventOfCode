function getarray(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    a = []
    open(filepath) do file
        for ln in eachline(file)
            push!(a,[])
            for char in ln
                push!(a[end],parse(Int,char))
            end
        end
    end
    return mapreduce(permutedims,vcat,a)
end

function argminunvisited(D,B)
    m = D[findfirst(B)]+1
    j = CartesianIndex(0,0,0,0)
    for i in CartesianIndices(B)
        B[i] && D[i] < m && (m = D[i]; j = i)
    end
    return j
end

function argminkey(d)
    minkey = first(keys(d))
    minvalue = d[minkey]
    for (key, value) in d
        if value < minvalue
            minkey = key
            minvalue = value
        end
    end
    minkey
end

# function modifieddijkstra(blocks)
#     si = size(blocks,1)
#     sj = size(blocks,2)
#     visits = zeros(Int,si,sj)
#     Dists = sum(blocks)*ones(Int,si,sj)
#     Up = zeros(Int,si,sj)
#     UpDists = sum(blocks)*ones(Int,si,sj)
#     Right = zeros(Int,si,sj)
#     RightDists = sum(blocks)*ones(Int,si,sj)
#     RightDists[1,1] = 0
#     Down = zeros(Int,si,sj)
#     DownDists = sum(blocks)*ones(Int,si,sj)
#     DownDists[1,1] = 0
#     Left = zeros(Int,si,sj)
#     LeftDists = sum(blocks)*ones(Int,si,sj)
#     while visits[si,sj] < 2
#         nv = argminunvisited(Dists,visits .< 2)
#         # println(nv, " ",Dists[nv])
#         up = Up[nv]
#         right = Right[nv]
#         down = Down[nv]
#         left = Left[nv]
#         uv = CartesianIndex(nv[1]-1,nv[2])
#         rv = CartesianIndex(nv[1],nv[2]+1)
#         dv = CartesianIndex(nv[1]+1,nv[2])
#         lv = CartesianIndex(nv[1],nv[2]-1)
#         if nv[1] != 1 && visits[uv] < 2 && up < 3    ### up ###
#             d = min(UpDists[nv],RightDists[nv],DownDists[nv],LeftDists[nv])
#             if blocks[uv] + Dists[nv] < Dists[uv]
#                 Dists[uv] = blocks[uv] + Dists[nv]
#                 Up[uv] = up+1
#                 # Right[uv] = 0
#                 # Down[uv] = 0
#                 # Left[uv] = 0
#             end
#         end
#         if nv[2] != sj && visits[rv] < 2 && right < 3    ### Right ###
#             if blocks[rv] + Dists[nv] < Dists[rv]
#                 Dists[rv] = blocks[rv] + Dists[nv]
#                 Right[rv] = right+1
#                 Up[rv] = 0
#                 Down[rv] = 0
#                 Left[rv] = 0
#             end
#         end
#         if nv[1] != si && visits[dv] < 2 && down < 3    ### Down ###
#             if blocks[dv] + Dists[nv] < Dists[dv]
#                 Dists[dv] = blocks[dv] + Dists[nv]
#                 Down[dv] = down+1
#                 Up[dv] = 0
#                 Right[dv] = 0
#                 Left[dv] = 0
#             end
#         end
#         if nv[2] != 1 && visits[lv] < 2 && left < 3    ### Left ###
#             if blocks[lv] + Dists[nv] < Dists[lv]
#                 Dists[lv] = blocks[lv] + Dists[nv]
#                 Left[lv] = left+1
#                 Up[lv] = 0
#                 Right[lv] = 0
#                 Down[lv] = 0
#             end
#         end
#         visits[nv] += 1
#     end
#     return Dists[si,sj]
# end

function modifieddijkstra(blocks)
    si = size(blocks,1)
    sj = size(blocks,2)
    Dists = sum(blocks)*ones(Int,si,sj,3,4)
    Unvisited = ones(Bool,si,sj,3,4)
    Dists[1,1,1,2] = 0 ## Just have to be put in manually ##
    Dists[1,1,1,3] = 0 ## ############################### ##
    while any(Unvisited[si,sj,:,3] .| Unvisited[si,sj,:,2])
        cv = argminunvisited(Dists,Unvisited)
        # println(cv," ",Dists[cv])
        cv[4] == 1 ? (uv = CartesianIndex(cv[1]-1,cv[2],cv[3]+1,1)) : (uv = CartesianIndex(cv[1]-1,cv[2],1,1))
        cv[4] == 2 ? (rv = CartesianIndex(cv[1],cv[2]+1,cv[3]+1,2)) : (rv = CartesianIndex(cv[1],cv[2]+1,1,2))
        cv[4] == 3 ? (dv = CartesianIndex(cv[1]+1,cv[2],cv[3]+1,3)) : (dv = CartesianIndex(cv[1]+1,cv[2],1,3))
        cv[4] == 4 ? (lv = CartesianIndex(cv[1],cv[2]-1,cv[3]+1,4)) : (lv = CartesianIndex(cv[1],cv[2]-1,1,4))
        if cv[1] != 1 && cv[4] != 3 && !(cv[4] == 1 && cv[3] >= 3)    ### up ###
            new = blocks[uv[1],uv[2]] + Dists[cv]
            if new < Dists[uv]
                Dists[uv] = new
                # if uv[1] == si && uv[2] == sj
                #     println("up $(rv[3]) - ",new)
                # end
            end
        end
        if cv[2] != sj && cv[4] != 4 && !(cv[4] == 2 && cv[3] >= 3)    ### right ###
            new = blocks[rv[1],rv[2]] + Dists[cv]
            if new < Dists[rv]
                Dists[rv] = new
                # if rv[1] == si && rv[2] == sj
                #     println("right $(rv[3]) - ",new)
                # end
            end
        end
        if cv[1] != si && cv[4] != 1 && !(cv[4] == 3 && cv[3] >= 3)    ### down ###
            new = blocks[dv[1],dv[2]] + Dists[cv]
            if new < Dists[dv]
                Dists[dv] = new
                # if dv[1] == si && dv[2] == sj
                #     println("down $(rv[3]) - ",new)
                # end
            end
        end
        if cv[2] != 1 && cv[4] != 2 && !(cv[4] == 4 && cv[3] >= 3)    ### left ###
            new = blocks[lv[1],lv[2]] + Dists[cv]
            if new < Dists[lv]
                Dists[lv] = new
                # if lv[1] == si && lv[2] == sj
                #     println("left $(rv[3]) - ",new)
                # end
            end
        end
        Unvisited[cv] = false
    end
    # println(sum(Unvisited))
    # display(Dists[1,3,:,:])
    return min(minimum(Dists[si,sj,:,3]),minimum(Dists[si,sj,:,2]))
end

function ultramodifieddijkstra(blocks)
    si = size(blocks,1)
    sj = size(blocks,2)
    Dists = sum(blocks)*ones(Int,si,sj,10,4)
    Unvisited = ones(Bool,si,sj,10,4)
    Dists[1,2,1,2] = blocks[1,2]
    Dists[2,1,1,3] = blocks[2,1]
    Q = Dict(CartesianIndex(1,2,1,2)=>blocks[1,2],CartesianIndex(2,1,1,3)=>blocks[2,1])
    while all(Unvisited[si,sj,:,3] .& Unvisited[si,sj,:,2])
        cv = argminkey(Q)
        cv[4] == 1 ? (uv = CartesianIndex(cv[1]-1,cv[2],cv[3]+1,1)) : (uv = CartesianIndex(cv[1]-1,cv[2],1,1))
        cv[4] == 2 ? (rv = CartesianIndex(cv[1],cv[2]+1,cv[3]+1,2)) : (rv = CartesianIndex(cv[1],cv[2]+1,1,2))
        cv[4] == 3 ? (dv = CartesianIndex(cv[1]+1,cv[2],cv[3]+1,3)) : (dv = CartesianIndex(cv[1]+1,cv[2],1,3))
        cv[4] == 4 ? (lv = CartesianIndex(cv[1],cv[2]-1,cv[3]+1,4)) : (lv = CartesianIndex(cv[1],cv[2]-1,1,4))
        if cv[1] != 1 && cv[4] != 3 && uv[3] <= 10 && (cv[4] == 1 || (cv[1] > 4 && cv[3] >= 4)) && Unvisited[uv]   ### up ###
            new = blocks[uv[1],uv[2]] + Dists[cv]
            if new < Dists[uv]
                Dists[uv] = new
                Q[uv] = new
            end
        end
        if cv[2] != sj && cv[4] != 4 && rv[3] <= 10 && (cv[4] == 2 || (cv[2] <= sj - 4 && cv[3] >= 4)) && Unvisited[rv]  ### right ###
            new = blocks[rv[1],rv[2]] + Dists[cv]
            if new < Dists[rv]
                Dists[rv] = new
                Q[rv] = new
            end
        end
        if cv[1] != si && cv[4] != 1 && dv[3] <= 10 && (cv[4] == 3 || (cv[1] <= si - 4 && cv[3] >= 4)) && Unvisited[dv]   ### down ###
            new = blocks[dv[1],dv[2]] + Dists[cv]
            if new < Dists[dv]
                Dists[dv] = new
                Q[dv] = new
            end
        end
        if cv[2] != 1 && cv[4] != 2 && lv[3] <= 10 && (cv[4] == 4 || (cv[2] > 4 && cv[3] >= 4)) && Unvisited[lv]  ### left ###
            new = blocks[lv[1],lv[2]] + Dists[cv]
            if new < Dists[lv]
                Dists[lv] = new
                Q[lv] = new
            end
        end
        Unvisited[cv] = false
        delete!(Q,cv)
    end
    return min(minimum(Dists[si,sj,:,3]),minimum(Dists[si,sj,:,2]))
end

function main(filename)
    blocks = getarray(filename)
    ans = ultramodifieddijkstra(blocks)
    write("/home/laurence/nextcloud/AOC23/day17/$(filename[1:end-4])output.txt","$ans")
    ans
end

@time main("input.txt")

# blocks = getarray("input.txt")
# @time ultramodifieddijkstra(blocks)
# @time modifieddijkstra(blocks)