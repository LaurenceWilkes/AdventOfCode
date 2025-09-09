function getarray(filename)
    filepath = joinpath(dirname(@__FILE__), "$(filename).txt")
    particles = []
    open(filepath) do file
        for ln in eachline(file)
            Line = []
            for m in eachmatch(r"-?\d+",ln)
                push!(Line,parse(Int,m.match))
            end
            push!(particles,[[Line[1],Line[2],Line[3]],[Line[4],Line[5],Line[6]]])
        end
    end
    return particles
end

function pathcross2d(a,b)
    va = a[2][1:2]
    vb = b[2][1:2]
    pa = a[1][1:2]
    pb = b[1][1:2]
    mat = hcat(vb,-va)
    s,t = (mat \ (pa-pb))
    s ≥ 0 && t ≥ 0 && return pb + s .* vb
    # println(pb .+ s .* vb)
    # println(pa .+ t .* va)
    return false
end

function particlepairs(filename,lb,ub)
    particles = getarray(filename)
    lp = length(particles)
    S = 0
    for i in 1:lp-1
        for j in i+1:lp
            try
                p = pathcross2d(particles[i],particles[j])
                lb ≤ p[1] ≤ ub && lb ≤ p[2] ≤ ub && (S += 1)
            catch
                continue
            end
        end
    end
    return S
end

# function objective_eq(p, v, t, P, V)
#     O = []
#     for i in eachindex(P)
#         push!(O, P[i][1] - p[1] + t[i] * (V[i][1] - v[1]))
#         push!(O, P[i][2] - p[2] + t[i] * (V[i][2] - v[2]))
#         push!(O, P[i][3] - p[3] + t[i] * (V[i][3] - v[3]))
#     end
#     return sqrt(sum(O .^ 2))
# end

# function objective_eq(v, t, P, V)
#     O = []
#     for i in 1:length(P)-1
#         push!(O, P[i][1] - P[i+1][1] + t[i+1] * (v[1] - V[i+1][1]) - t[i] * (v[1] - V[i][1]))
#         push!(O, P[i][2] - P[i+1][2] + t[i+1] * (v[2] - V[i+1][2]) - t[i] * (v[2] - V[i][2]))
#         push!(O, P[i][3] - P[i+1][3] + t[i+1] * (v[3] - V[i+1][3]) - t[i] * (v[3] - V[i][3]))
#     end
#     push!(O, P[end][1] - P[1][1] + t[1] * (v[1] - V[1][1]) - t[end] * (v[1] - V[end][1]))
#     push!(O, P[end][2] - P[1][2] + t[1] * (v[2] - V[1][2]) - t[end] * (v[2] - V[end][2]))
#     push!(O, P[end][3] - P[1][3] + t[1] * (v[3] - V[1][3]) - t[end] * (v[3] - V[end][3]))
#     return sqrt(sum(O .^ 2))
# end

# cross(a,b) = [a[2]*b[3]-a[3]*b[2],a[3]*b[1]-a[1]*b[3],a[1]*b[2]-a[2]*b[1]]

# crossop(a) = reshape([0,-a[3],a[2],a[3],0,-a[1],-a[2],a[1],0],3,3)

# triplewedge(a,b,c) = a[1]*b[2]*c[3] - a[2]*b[1]*c[3] - a[1]*b[3]*c[2] + a[3]*b[1]*c[2] + a[2]*b[3]*c[1] - a[3]*b[2]*c[1]

# wedgevec(a,b) = [a[2]*b[3]-a[3]*b[2],a[3]*b[1]-a[1]*b[3],a[1]*b[2]-a[2]*b[1]]'

wedge(a,b) = [a[2]*b[3]-a[3]*b[2],a[1]*b[3]-a[3]*b[1],a[1]*b[2]-a[2]*b[1]]

wedgemat(a) = reshape([0,a[3],a[2],a[3],0,-a[1],-a[2],-a[1],0],3,3)

function twoblock(A,B)
    C = zeros(Int, 6, 6)
    C[1:3,1:3] = A
    C[4:6,4:6] = B
    C
end

function solveeqs(P,V)
    lP = length(P)

    # a,b,c,d,e = 1,1,1,1,1
    # while a == b || b == c || c == d || d == e || a == c || b == d || c == e || d == a || e == b || a == e
    #     (a,b,c,d,e) = rand(1:length(P),5)
    # end
    # pairset = [(a,b),(b,c),(a,c),(a,d),(b,d),(c,d),(a,e),(b,e),(c,e),(d,e)]

    pairset = [(1,2),(2,3),(1,3)]

    # Mat = vcat([twoblock(wedgemat(V[i]-V[j]),wedgemat(P[j]-P[i])) for (i,j) in pairset]...)

    pvmats = [hcat(wedgemat(V[i]-V[j]),wedgemat(P[j]-P[i])) for (i,j) in pairset]
    resvecs = [wedge(P[j],V[j]) .- wedge(P[i],V[i]) for (i,j) in pairset]
    Mat = BigInt.(-vcat(pvmats...)) .// 1
    b = BigInt.(vcat(resvecs...)) .// 1
    # Mat \ b
    # display(Mat)
    inv(Mat' * Mat) * Mat' * b
end

# function solveeqs(P,V)
#     lP = length(P)

#     a,b,c,d,e = 1,1,1,1,1
#     while a == b || b == c || c == d || d == e || a == c || b == d || c == e || d == a || e == b || a == e
#         (a,b,c,d,e) = rand(1:length(P),5)
#     end
#     pairset = [(a,b),(b,c),(a,c),(a,d),(b,d),(c,d),(a,e),(b,e),(c,e),(d,e)]

#     # pairset = [(1,2),(2,3),(1,3)]

#     pvvecs = [wedgevec(P[j]-P[i],V[j]-V[i]) for (i,j) in pairset]
#     resvecs = [triplewedge(P[i],P[j],V[i]-V[j]) for (i,j) in pairset]
#     Mat = BigInt.(vcat(pvvecs...)) .// 1
#     b = BigInt.(vcat(resvecs...)) .// 1
#     # Mat \ b
#     # display(Mat)
#     inv(Mat' * Mat) * Mat' * b
# end

function rockthrow(filename)
    particles = getarray(filename)
    P = [p[1] for p in particles]
    V = [p[2] for p in particles]
    p = solveeqs(P,V)
    println(p)
    sum(p[1:3])
end

# function rockthrow(filename)
#     particles = getarray(filename)
#     P = [p[1] for p in particles]
#     V = [p[2] for p in particles]
#     initial_guess = zeros(length(P) + 3)
#     initial_guess[1:3] .= -1

#     lowbounds = zeros(length(P) + 3)

#     result = nlsolve(x -> objective_eq(x[1:3], x[4:end], P, V), initial_guess, lower = lowbounds)
#     println("Solution: ", result.zero)
# end

# @time particlepairs("eg",7,27)
# @time particlepairs("input",200000000000000,400000000000000)


@time rockthrow("input")

# pathcross2d([[19,13],[-2,1]],[[18,19],[-1,-1]])