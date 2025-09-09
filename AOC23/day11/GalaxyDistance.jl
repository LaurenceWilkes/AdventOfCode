function getarray(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    Universe = []
    open(filepath) do file
        i = 1
        for ln in eachline(file)
            push!(Universe,[])
            for char in ln
                push!(Universe[i],char)
            end
            i += 1
        end
    end
    return mapreduce(permutedims, vcat, Universe)
end

function expandinds(Universe)
    jexpandinds = []
    iexpandinds = []
    for (i,r) in enumerate(eachrow(Universe))
        all(r .== '.') && push!(iexpandinds,i)
    end
    for (j,c) in enumerate(eachcol(Universe))
        all(c .== '.') && push!(jexpandinds,j)
    end
    return iexpandinds,jexpandinds
end

function distbetween(a,b,iexpandinds,jexpandinds,expandfactor = 1000000)
    ab2 = abs(b[2]-a[2])
    ab1 = abs(b[1]-a[1])
    dist = ab1+ab2
    mab2 = min(a[2],b[2])
    mab1 = min(a[1],b[1])
    for j in jexpandinds
        if j in mab2:mab2+ab2
            dist += expandfactor-1
        end
    end
    for i in iexpandinds
        if i in mab1:mab1+ab1
            dist += expandfactor-1
        end
    end
    return dist
end

function getgalaxies(Universe)
    gals = []
    for i in axes(Universe,2), j in axes(Universe,1)
        Universe[i,j] == '#' && push!(gals,(i,j))
    end
    return gals
end

function distlist(gals,iexpandinds,jexpandinds)
    dists = []
    lg = length(gals)
    for i in 1:lg-1
        for j in i+1:lg
            push!(dists,distbetween(gals[i],gals[j],iexpandinds,jexpandinds))
            # println(gals[i]," ", gals[j], " ",distbetween(gals[i],gals[j],iexpandinds,jexpandinds))
        end
    end
    return dists
end

function distsum(filename)
    Universe = getarray(filename)
    iexpandinds,jexpandinds = expandinds(Universe)
    gals = getgalaxies(Universe)
    dists = distlist(gals,iexpandinds,jexpandinds)
    println(sum(dists))
end

@time distsum("input.txt")

# display(getarray("eg.txt")) # good
# println(expandinds(getarray("eg.txt")))