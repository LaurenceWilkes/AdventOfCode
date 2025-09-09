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
                    push!(a[end],char)
                end
            end
        end
        push!(Blocks,mapreduce(permutedims,vcat,a))
    end
    return Blocks
end

function colsymmetry(block)
    lines = Set()
    firstcol = block[:,1]           ## left ##
    for j in 2:2:size(block,2)
        symflag = true
        if block[:,j] == firstcol
            for k in 2:j÷2
                block[:,k] != block[:,j-k+1] && (symflag = false; break)
            end
            symflag == true && push!(lines,j÷2)
        end
    end
    lastcol = block[:,end]           ## right ##
    for j in size(block,2)-1:-2:1
        symflag = true
        if block[:,j] == lastcol
            for k in 1:(size(block,2)-j+1)÷2
                block[:,j+k] != block[:,end-k] && (symflag = false; break)
            end
            symflag == true && push!(lines,size(block,2)-(size(block,2)-j+1)÷2)
        end
    end
    return lines
end

function rowsymmetry(block)
    lines = Set()
    firstrow = block[1,:]           ## up ##
    for j in 2:2:size(block,1)
        symflag = true
        if block[j,:] == firstrow
            for k in 2:j÷2
                block[k,:] != block[j-k+1,:] && (symflag = false; break)
            end
            symflag == true && push!(lines,j÷2)
        end
    end
    lastrow = block[end,:]           ## down ##
    for j in size(block,1)-1:-2:1
        symflag = true
        if block[j,:] == lastrow
            for k in 1:(size(block,1)-j+1)÷2
                block[j+k,:] != block[end-k,:] && (symflag = false; break)
            end
            symflag == true && push!(lines,size(block,1)-(size(block,1)-j+1)÷2)
        end
    end
    return lines
end

# function blocksymmetry(b)
#     S = 0
#     cs = colsymmetry(b)
#     if cs != false
#         for s in cs
#             S += s
#         end
#     end
#     rs = rowsymmetry(b)
#     if rs != false
#         for r in rs
#             S += 100*r
#         end
#     end
#     return S
# end

function SymmetrySummary(filename)
    Blocks = getarray(filename)
    S = 0
    for (i,b) in enumerate(Blocks)
        cs = colsymmetry(b)
        if !isempty(cs)
            for s in cs
                S += s
            end
        end
        rs = rowsymmetry(b)
        if !isempty(rs)
            for r in rs
                S += 100*r
            end
        end
    end
    println(S)
end

@time SymmetrySummary("input.txt")