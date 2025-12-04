using Mods, BenchmarkTools

function followpath(RLinstructions,nodeinstructions)
    lRL = length(RLinstructions)
    i = 0
    currentloc = "AAA"
    while currentloc != "ZZZ"
        i += 1
        RLinstructions[mod1(i,lRL)] == 'L' && (currentloc = nodeinstructions[currentloc][1])
        RLinstructions[mod1(i,lRL)] == 'R' && (currentloc = nodeinstructions[currentloc][2])
    end
    return i
end

function NavigateDesert(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    open(filepath) do file
        RLinstructions = readline(file)
        readline(file);
        nodeinstructions = Dict()
        while !eof(file)
            ln = readline(file)
            loc, r, l = match(r"(?<loc>[A-Z]{3}) = \((?<l>[A-Z]{3}), (?<r>[A-Z]{3})\)",ln)
            nodeinstructions[loc] = (r,l)
        end
        println("Number of steps is: ", followpath(RLinstructions,nodeinstructions))
    end
end

function followmultiplepath!(currentnodes,RLinstructions,nodeinstructions)
    lRL = length(RLinstructions)
    i = 0
    while any(map(x->x[3],currentnodes) .!= 'Z')
        i += 1
        for j in eachindex(currentnodes)
            RLinstructions[mod1(i,lRL)] == 'L' && (currentnodes[j] = nodeinstructions[currentnodes[j]][1])
            RLinstructions[mod1(i,lRL)] == 'R' && (currentnodes[j] = nodeinstructions[currentnodes[j]][2])
        end
    end
    return i
end

function followmultiplepath!(currentnodes,RLinstructions,nodeinstructions)
    lRL = length(RLinstructions)
    i = 0
    while any(map(x->x[3],currentnodes) .!= 'Z')
        i += 1
        for j in eachindex(currentnodes)
            RLinstructions[mod1(i,lRL)] == 'L' && (currentnodes[j] = nodeinstructions[currentnodes[j]][1])
            RLinstructions[mod1(i,lRL)] == 'R' && (currentnodes[j] = nodeinstructions[currentnodes[j]][2])
        end
    end
    return i
end

function disttoz(node,RLinstructions,nodeinstructions)
    lRL = length(RLinstructions)
    i = 0
    while node[3] != 'Z' && i != lRL
        i += 1
        RLinstructions[mod1(i,lRL)] == 'L' && (currentloc = nodeinstructions[currentloc][1])
        RLinstructions[mod1(i,lRL)] == 'R' && (currentloc = nodeinstructions[currentloc][2])
    end
    return i
end

function loopfinder(node,RLinstructions,nodeinstructions)
    i = 1
    lRL = length(RLinstructions)
    currentloc = node
    mi = 1
    looppoint = 0
    scaledlooplength = 0
    visitedlocs = [currentloc]
    breakflag = false
    Zs = []
    while !breakflag
        RLinstructions[mi] == 'L' ? (currentloc = nodeinstructions[currentloc][1]) : (currentloc = nodeinstructions[currentloc][2])
        push!(visitedlocs,currentloc)
        i += 1
        mi = mod1(i,lRL)
        for j in mi:lRL:i-lRL
            visitedlocs[j] == currentloc && (breakflag = true; looppoint = j; scaledlooplength = (i-j)÷lRL; break)
        end
        currentloc[3] == 'Z' && push!(Zs,(currentloc,i))
    end
    return [z[2] for z in Zs if z[2]>=looppoint], looppoint, scaledlooplength
end

function minzpoint(currentnodes,RLinstructions,nodeinstructions)

end

function GhostsNavigateDesert(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    open(filepath) do file
        RLinstructions = readline(file)
        readline(file);
        nodeinstructions = Dict()
        currentnodes = []
        while !eof(file)
            ln = readline(file)
            loc, r, l = match(r"(?<loc>[A-Z]{3}) = \((?<l>[A-Z]{3}), (?<r>[A-Z]{3})\)",ln)
            loc[3] == 'A' && push!(currentnodes,loc)
            nodeinstructions[loc] = (r,l)
        end
        K = [k for k in keys(nodeinstructions) if k[3] == 'A']
        Arr = []
        for k in K
            lf = loopfinder(k,RLinstructions,nodeinstructions)
            println(lf)
            push!(Arr,loopfinder(k,RLinstructions,nodeinstructions)[3])
        end
        println(lcm(Arr...,length(RLinstructions)))
        # println(length(RLinstructions))
        # println(loopfinder("LBA",RLinstructions,nodeinstructions))
        # println("Number of steps is: ", fmp)
        # write("/home/laurence/nextcloud/AOC23/day8/$(filename[1:end-4])output.txt","$fmp")
    end
end

# @time NavigateDesert("input.txt")
@time GhostsNavigateDesert("input.txt")
# @time NavigateDesert("eg.txt")
