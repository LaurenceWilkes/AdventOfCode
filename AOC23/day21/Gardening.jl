function getarray(filename)
    filepath = joinpath(dirname(@__FILE__), "$(filename).txt")
    a = []
    sloc = CartesianIndex(0,0)
    open(filepath) do file
        i = 0
        for ln in eachline(file)
            i+=1
            push!(a,[])
            j = 0
            for char in ln
                j += 1
                push!(a[end],char)
                if char == 'S'
                    sloc = CartesianIndex(i,j)
                end
            end
        end
    end
    return sloc, mapreduce(permutedims,vcat,a)
end

function stepreach(loc,steps,map)
    up = loc + CartesianIndex(1,0)
    down = loc + CartesianIndex(-1,0)
    left = loc + CartesianIndex(0,-1)
    right = loc + CartesianIndex(0,1)
    reachsteps = Set()
    if steps == 1
        map[up] != '#' && push!(reachsteps,up)
        map[down] != '#' && push!(reachsteps,down)
        map[left] != '#' && push!(reachsteps,left)
        map[right] != '#' && push!(reachsteps,right)
    else
        map[up] != '#' && push!(reachsteps,stepreach(up,steps-1,map)...)
        map[down] != '#' && push!(reachsteps,stepreach(down,steps-1,map)...)
        map[left] != '#' && push!(reachsteps,stepreach(left,steps-1,map)...)
        map[right] != '#' && push!(reachsteps,stepreach(right,steps-1,map)...)
    end
    return reachsteps
end

function lazyvisited(steps,sloc,map)
    up = CartesianIndex(1,0)
    down = CartesianIndex(-1,0)
    left = CartesianIndex(0,-1)
    right = CartesianIndex(0,1)
    visited = Dict{Int,Set}(0=>Set([sloc]))
    for s in 1:steps
        visited[s] = Set()
        for v in visited[s-1]
            uv = v + up
            dv = v + down
            lv = v + left
            rv = v + right
            map[mod1.(uv[1],131),mod1.(uv[2],131)] != '#' && push!(visited[s],uv)
            map[mod1.(dv[1],131),mod1.(dv[2],131)] != '#' && push!(visited[s],dv)
            map[mod1.(lv[1],131),mod1.(lv[2],131)] != '#' && push!(visited[s],lv)
            map[mod1.(rv[1],131),mod1.(rv[2],131)] != '#' && push!(visited[s],rv)
        end
    end
    return visited[steps]
end

function countreach(steps,filename)
    sloc, map = getarray(filename)
    length(lazyvisited(steps,sloc,map))
end

x1 = 65
S = 131
y1 = countreach(x1,"input")
y2 = countreach(x1+S,"input")
y3 = countreach(x1+2S,"input")
y4 = countreach(x1+3S,"input")
y4 = countreach(x1+4S,"input")

# println(y1)
# println(y2)
# println(y3)
# println(y4)

# A = (y3+y1)/2-y2
# B = (y3-y1)/2
# C = y2

# x = (26501365-65)÷262
# println(A*x^2+B*x+C)
