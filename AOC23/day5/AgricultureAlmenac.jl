struct seedmap
    from::String
    to::String
    map::Vector{Vector{Int}}
end

function concatenatemap(ma,mb)
    for la in ma
        for lb in mb
            afirststart = la[2]
            afirststop = la[2]+la[3]-1
            asecondstart = la[1]
            asecondstop = la[1]+la[3]-1
            bfirststart = lb[2]
            bfirststop = lb[2]+lb[3]-1
            bsecondstart = lb[1]
            bsecondstop = lb[1]+lb[3]-1
            if asecondstop < bfirstsart
                push!(output,r)
            elseif asecondstart <= bfirststart < asecondstop <  bfirststop
                mapstart != r.start && push!(output,r.start:mapstart-1)
                push!(output,mapstart+l[1]-l[2]:r.stop+l[1]-l[2])
            elseif bfirststart < asecondstart && asecondstop <  bfirststop
                mapstart != r.start && push!(output,r.start:mapstart-1)
                mapstop != r.stop && push!(output,mapstop+1:r.stop)
                push!(output,mapstart+l[1]-l[2]:mapstop+l[1]-l[2])
            elseif asecondstart <= bfirststart && bfirststop <= asecondstop
                mapstop != r.stop && push!(output,mapstop+1:r.stop)
                push!(output,r.start+l[1]-l[2]:mapstop+l[1]-l[2])
            elseif bfirststart < asecondstart < bfirststop <= asecondstop
                push!(output,r)
            elseif bfirstsart < asecondstop

            end
        end
    end
end

function applymap(seed,map)
    for l in map
        seed in l[2]:l[2]+l[3]-1 && return seed + l[1] - l[2]
    end
end

function applymap(range::UnitRange,map)
    for l in map
        seed in l[2]:l[2]+l[3]-1 && return seed + l[1] - l[2]
    end
end

# function applymap!(r::UnitRange,map)
#     output = [r]
#     for l in map
#         mapstart = l[2]
#         mapstop = l[2]+l[3]-1
#         if mapstart > r.stop
#             push!(output,r)
#         elseif mapstart in r && mapstop > r.stop
#             mapstart != r.start && push!(output,r.start:mapstart-1)
#             push!(output,mapstart+l[1]-l[2]:r.stop+l[1]-l[2])
#         elseif mapstart in r && mapstop in r
#             mapstart != r.start && push!(output,r.start:mapstart-1)
#             mapstop != r.stop && push!(output,mapstop+1:r.stop)
#             push!(output,mapstart+l[1]-l[2]:mapstop+l[1]-l[2])
#         elseif mapstart < r.start && mapstop in r
#             mapstop != r.stop && push!(output,mapstop+1:r.stop)
#             push!(output,r.start+l[1]-l[2]:mapstop+l[1]-l[2])
#         elseif mapstop < r.start
#             push!(output,r)
#         end
#         seed in l[2]:l[2]+l[3]-1 && return seed + l[1] - l[2]
#     end
# end

function mapseed(seed,seedmaps)
    for sm in seedmaps
        for l in sm.map
            seed in l[2]:l[2]+l[3]-1 && (seed += l[1] - l[2]; break)
        end
    end
    return seed
end

function inverseseedmap(location,seedmaps)
    # println()
    # println(location)
    for i in eachindex(seedmaps)
        for l in seedmaps[length(seedmaps)-i+1].map
            location in l[1]:l[1]+l[3]-1 && (location += l[2] - l[1]; break)
        end
        # println(location)
    end
    return location
end

function produceseedmaps(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    seednumbers = []
    seedmaps = []
    open(filepath) do file
        seedline = readline(file)
        for m in eachmatch(r"\d+",seedline)
            push!(seednumbers,parse(Int,m.match))
        end
        readline(file);
        while !eof(file)
            fromtoline = readline(file)
            from,to = match(r"(?<from>[a-zA-Z]+)-to-(?<to>[a-zA-Z]+)",fromtoline)
            DSR = []
            while true
                ln = readline(file)
                (ln == "" || eof(file)) && break
                dsr = []
                for m in eachmatch(r"\d+",ln)
                    push!(dsr,parse(Int,m.match))
                end
                push!(DSR,dsr)
            end
            push!(seedmaps,seedmap(from,to,DSR))
        end
    end
    return seednumbers, seedmaps
end

function minimumlocation(filename)
    seednumbers, seedmaps = produceseedmaps(filename)
    seeddestinations = []
    for seed in seednumbers
        push!(seeddestinations,mapseed(seed,seedmaps))
    end
    println(minimum(seeddestinations))
end

function inseednumbers(n,seednumbers)
    for i in 1:length(seednumbers)÷2
        startseed = seednumbers[2i-1]
        endseed = seednumbers[2i-1]+seednumbers[2i]-1
        n in startseed:endseed && return true
    end
    return false
end

function trueminimumlocation(filename)
    seednumbers,seedmaps = produceseedmaps(filename)
    startloc = 0
    while true
        inseednumbers(inverseseedmap(startloc,seedmaps),seednumbers) && (println(startloc); break)
        startloc += 1
        mod(startloc,1e7) == 0 && println(startloc, " - done")
    end
end

# @time minimumlocation("eg.txt")
@time trueminimumlocation("input.txt")