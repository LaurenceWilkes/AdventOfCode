function getarray(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    Blocks = []
    a = []
    open(filepath) do file
        for ln in eachline(file)
            if isempty(ln)
                push!(Blocks,mapreduce(permutedims,vcat,a))
                a = []
            else
                push!(a,[])
                for char in ln
                    char == '#' && push!(a[end],true)
                    char == '.' && push!(a[end],false)
                end
            end
        end
        push!(Blocks,mapreduce(permutedims,vcat,a))
    end
    return Blocks
end

leftimperfection(block,i) = sum(view(block,:,1:i) .⊻ view(block,:,2i:-1:i+1))
rightimperfection(block,i) = (sb = size(block,2); sum(view(block,:,sb-i+1:sb) .⊻ view(block,:,sb-i:-1:sb-2i+1)))
upimperfection(block,i) = sum(view(block,1:i,:) .⊻ view(block,2i:-1:i+1,:))
downimperfection(block,i) = (sb = size(block,1); sum(view(block,sb-i+1:sb,:) .⊻ view(block,sb-i:-1:sb-2i+1,:)))

function imperfectsymmetry(block)
    scol = size(block,2)
    srow = size(block,1)
    for i in 1:scol÷2
        leftimperfection(block,i) == 1 && return i
        rightimperfection(block,i) == 1 && return scol-i
    end
    for i in 1:srow÷2
        upimperfection(block,i) == 1 && return 100*i
        downimperfection(block,i) == 1 && return 100*(srow-i)
    end
end

function SymmetrySummary(filename)
    Blocks = getarray(filename)
    S = 0
    for (i,b) in enumerate(Blocks)
        # i != 2 && continue
        S += imperfectsymmetry(b)
    end
    println(S)
end

@time SymmetrySummary("input.txt")