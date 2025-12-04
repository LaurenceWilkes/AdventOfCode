function cardscore(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    open(filepath) do file
        winningcounts = []
        for ln in eachline(file)
            splitind = findfirst(r"\|",ln).start
            cardnoind = findfirst(r"\:",ln).start
            # println(splitind,",",cardnoind)
            WN = []
            YN = []
            for m in eachmatch(r"\d+",SubString(ln,cardnoind+1:splitind))
                push!(WN,parse(Int,m.match))
            end
            for m in eachmatch(r"\d+",SubString(ln,splitind:length(ln)))
                push!(YN,parse(Int,m.match))
            end
            # println(YN)
            # println(WN)
            # println()
            MatchingNos = []
            for yn in YN
                (yn in WN) && push!(MatchingNos,yn)
            end
            push!(winningcounts,length(MatchingNos))
        end
        println(sum(2^(wc-1) for wc in winningcounts if wc != 0))
    end
end

function wins(ln)
    splitind = findfirst(r"\|",ln).start
    cardnoind = findfirst(r"\:",ln).start
    # println(splitind,",",cardnoind)
    WN = []
    YN = []
    for m in eachmatch(r"\d+",SubString(ln,cardnoind+1:splitind))
        push!(WN,parse(Int,m.match))
    end
    for m in eachmatch(r"\d+",SubString(ln,splitind:length(ln)))
        push!(YN,parse(Int,m.match))
    end
    # println(YN)
    # println(WN)
    # println()
    MatchingNos = []
    for yn in YN
        (yn in WN) && push!(MatchingNos,yn)
    end
    return length(MatchingNos)
end

function truecardscore(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    Lines = []
    open(filepath) do file
        for ln in eachline(file)
            push!(Lines,ln)
        end
    end
    lL = length(Lines)
    copymultiplier = ones(Int,lL)
    for (i,ln) in enumerate(Lines)
        for j in 1:wins(ln)
            i+j > lL && break
            copymultiplier[i+j] += copymultiplier[i]
        end
    end
    println(sum(copymultiplier))
end

@time truecardscore("input.txt")