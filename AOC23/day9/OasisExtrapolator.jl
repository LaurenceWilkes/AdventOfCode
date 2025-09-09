using Polynomials

function differences!(seq)
    initialterms = []
    j = length(seq)
    while !all(seq .== 0)
        pval = seq[1]
        for i in 2:j
            val = seq[i]
            seq[i] -= pval
            pval = val
        end
        push!(initialterms,popfirst!(seq))
        j -= 1
    end
    return initialterms
end

constructmatrix(d) = [binomial(d+1-j,i>=j ? i-j+1 : -1)//1 for i in 0:d, j in 0:d]

function ProduceNthTerm(initialterms)
    b = []
    lit = length(initialterms)
    push!(b,initialterms[lit])
    for i in 1:lit-1
        b = constructmatrix(i-1) \ b
        push!(b,initialterms[lit-i])
    end
    return Polynomial(reverse(b))
end

function nextelement(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    open(filepath) do file
        finalvals = []
        for ln in eachline(file)
            sequence = []
            for m in eachmatch(r"-?\d+",ln)
                push!(sequence,parse(Int,m.match))
            end
            lseq = length(sequence)
            p = ProduceNthTerm(differences!(sequence))
            push!(finalvals,p(lseq)) ## Change lseq to -1 for part 1
        end
        println(sum(finalvals))
    end
end

@time nextelement("input.txt")

# seq = [0,3,6,9,12,15]
# @time println(differences!(seq))

