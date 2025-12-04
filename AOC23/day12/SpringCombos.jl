function isfit(diagram,ranges)
    for (i,c) in enumerate(diagram)
        breakflag = false
        if c == '#'
            for r in ranges
                (i in r[1]:r[2]) && (i != r[2]) && (breakflag = true; break)
            end
            !breakflag && return false
        elseif c == '.'
            for r in ranges
                (i in r[1]:r[2]) && (i != r[2]) && return false
            end
        end
    end
    return true
end

function combocount(diagram,pattern)
    ld = length(diagram)
    ranges = []
    prevend = 0
    for p in pattern
        newend = prevend+p+1
        push!(ranges,[prevend+1,newend])
        prevend = newend
    end
    lp = length(pattern)
    cri = lp
    poss = 0
    movedflag = false
    isfit(diagram,ranges) && (poss += 1)
    while cri > 0
        if ranges[cri][2] >= ld+1 || (cri != lp && ranges[cri][2]+1 >= ranges[cri+1][1])
            cri -= 1
            movedflag = true
            continue
        end
        if movedflag
            ranges[cri] .+= 1
            ranges[cri+1] .-= ranges[cri+1][1] - ranges[cri][2] - 1
            for i in cri+2:lp
                ranges[i] .-= ranges[i][1] - ranges[i-1][2] - 1
            end
            isfit(diagram,ranges) && (poss += 1)
            movedflag = false
            cri = lp
            continue
        end
        ranges[cri] .+= 1
        isfit(diagram,ranges) && (poss += 1)
    end
    return poss
end


function sumspringcombos(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    rowcombos = []
    open(filepath) do file
        counter = 2
        for ln in eachline(file)
            diagram = match(r"[.#?]+",ln).match
            diagram = ((diagram*"?")^4)*diagram
            pattern = repeat([parse(Int,m.match) for m in eachmatch(r"\d+",ln)],5)
            push!(rowcombos,combocount(diagram,pattern))
            counter -= 1
            counter == 0 && break
        end
    end
    println(sum(rowcombos))
end

@time println(combocount((".??..??...?##."*"?")^4*".??..??...?##.", repeat([1,1,3],5)))

# @time isfit("???.###",[[1, 2], [3, 4], [5, 8]])

# @time sumspringcombos("eg.txt")