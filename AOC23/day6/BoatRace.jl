include("input.jl")
#`input.jl`
#T = [x     x     x     x]
#D = [x     x     x     x]

function main()
    combinations = 1
    for (i,t) in enumerate(T)
        waystowin = 0
        for h in 0:(t-1)
            (t-h)*h > D[i] && (waystowin += 1)
        end
        combinations *= waystowin
    end
    println(combinations)
end

function main2()
    t = 41968894
    d = 214178911271055
    waystowin = 0
    for h in 0:(t-1)
        (t-h)*h > d && (waystowin += 1)
    end
    println(waystowin)
end

@time main2()
