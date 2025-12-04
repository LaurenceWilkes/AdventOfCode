mutable struct Block
    inds::Vector{UnitRange}
end

function Base.getindex(B::Block, i)
    B.inds[i]
end

Base.length(B::Block) = B[1].stop-B[1].start+B[2].stop-B[2].start+B[3].stop-B[3].start

# Base.iterate(B::Block) = B.start, 1
# function Base.iterate(B::Block,state)
#     state > length(B) && return nothing
#     dir = B.stop .- B.start
#     nb = B.start .+ state .* dir .÷ norm(dir)
#     return nb, state+1
# end

# function upstanding(B::Block)
#     B[3].start != B[3].stop && return true
#     return false
# end

function axis(B::Block)
    B[3].start != B[3].stop && return 3
    B[2].start != B[2].stop && return 2
    B[1].start != B[1].stop && return 1
end

function zlinear(A::Block,B::Block)
    if length(intersect(A[1],B[1])) ≥ 1 && length(intersect(A[2],B[2])) ≥ 1
        return B[3].start - A[3].stop
    end
    return false
end

higher(A::Block,B::Block) = A[3].start < B[3].start

function getblocks(filename)
    filepath = joinpath(dirname(@__FILE__), "$(filename).txt")
    Blocks = []
    open(filepath) do file
        for ln in eachline(file)
            b = []
            for m in eachmatch(r"\d+",ln)
                push!(b,parse(Int,m.match))
            end
            push!(Blocks,Block([min(b[1],b[4]):max(b[1],b[4]),min(b[2],b[5]):max(b[2],b[5]),min(b[3],b[6]):max(b[3],b[6])]))
        end
    end
    return sort!(Blocks,by = b -> b[3].start)
end

function drop!(Blocks)
    Beneath = Dict()
    Above = Dict()
    for i in eachindex(Blocks)
        mindist = Blocks[i][3].start
        Mininds = []
        for j in i-1:-1:1
            zl = zlinear(Blocks[j],Blocks[i])
            if zl != false && zl ≤ mindist
                if zl == mindist
                    push!(Mininds,j)
                else
                    empty!(Mininds)
                    mindist = zl
                    push!(Mininds,j)
                end
            end
        end
        Blocks[i].inds[3] = Blocks[i][3] .- (mindist-1)
        Beneath[i] = copy(Mininds)
        for in in Mininds
            !haskey(Above,in) && (Above[in] = [])
            push!(Above[in],i)
        end
        !haskey(Above,i) && (Above[i] = [])
    end
    return Beneath, Above
end

function indieblocks(filename)
    Blocks = getblocks(filename)
    Beneath, Above = drop!(Blocks)
    S = 0
    for i in eachindex(Blocks)
        bflag = false
        for j in Above[i]
            if length(Beneath[j]) == 1
                bflag = true
                break
            end
        end
        !bflag && (S += 1)
    end
    return S
end

function depblocks(i,Beneath,Above)
    onblocks = Set([])
    fallblocks = Set([i])
    curblocks = Set([i])
    loopflag = true
    while !isempty(curblocks)
        loopflag = false
        for is in curblocks
            !isempty(Above[is]) && push!(onblocks,Above[is]...)
        end
        empty!(curblocks)
        for o in onblocks
            bflag = false
            for b in Beneath[o]
                !(b in fallblocks) && (bflag = true; break)
            end
            !bflag && (push!(fallblocks,o); push!(curblocks,o))
        end
        empty!(onblocks)
    end
    return fallblocks
end

function disintegrator(filename)
    Blocks = getblocks(filename)
    Beneath, Above = drop!(Blocks)
    S = 0
    for i in eachindex(Blocks)
        # println("$i gives $(length(depblocks(i,Beneath,Above)))")
        S += (length(depblocks(i,Beneath,Above))-1)
    end
    return S
end

@time disintegrator("input")