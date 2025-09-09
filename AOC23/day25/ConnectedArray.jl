using Random, BenchmarkTools

function getarray(filename)
    filepath = joinpath(dirname(@__FILE__), "$(filename).txt")
    vertices = Set()
    Neighbours = Dict()
    open(filepath) do file
        for ln in eachline(file)
            startvert = ln[1:3]
            !haskey(Neighbours,startvert) && (Neighbours[startvert] = [])
            push!(vertices,startvert)
            for m in eachmatch(r"[a-z]+",ln[5:end])
                !haskey(Neighbours,m.match) && (Neighbours[m.match] = [])
                push!(vertices,m.match)
                push!(Neighbours[m.match],startvert)
                push!(Neighbours[startvert],m.match)
            end
        end
    end
    return collect(vertices), Neighbours
end

function PP(V,N)
    Q = Dict()
    for (i,v) in enumerate(V)
        Q[v] = i
    end
    NN = []
    for v in V
        push!(NN,[])
        for w in N[v]
            push!(NN[end],Q[w])
        end
    end
    return length(V), NN
end

# function connections(V,A,N)
#     C = 0
#     for i in eachindex(V)
#         if A[i]
#             for w in N[V[i]]
#                 !A[w] && (C += 1)
#             end
#         end
#     end
#     return C
# end

# function findtriple!(V,N)
#     lV = length(V)
#     A = zeros(Bool,lV)
#     A[1:lV÷2] .= true
#     siV = collect(1:lV)
#     while connections(V,A,N) > 3
#         shuffle!(siV)
#         for i in siV

#         end
#     end
# end

function connections(A,N)
    C = 0
    for i in eachindex(A)
        if A[i]
            for w in N[i]
                !A[w] && (C += 1)
            end
        end
    end
    return C
end

function outedge(i,A,N)
    C = 0
    for w in N[i]
        !A[w] && (C += 1)
    end
    return C
end

function inedge(i,A,N)
    C = 0
    for w in N[i]
        A[w] && (C += 1)
    end
    return C
end

function prodtrue(A)
    T,F = 0,0
    for a in A
        if a == true
            T += 1
        else
            F += 1
        end
    end
    return T*F
end

function findtriple(lV,N)
    A = zeros(Bool,lV)
    A[1:lV÷2] .= true
    siV = collect(1:lV)
    lA = lV÷2
    while connections(A,N) > 3
        shuffle!(siV)
        for i in siV
            if A[i]
                2*outedge(i,A,N) >= length(N[i]) && lA > 1 && (A[i] = false; lA -= 1)
            else
                2*inedge(i,A,N) >= length(N[i]) && lA < lV && (A[i] = true; lA += 1)
            end
        end
    end
    return A
end

function main()
    V,N = getarray("input")
    lV, N = PP(V,N)
    @time A = findtriple(lV,N)
    println(prodtrue(A))
end

main()