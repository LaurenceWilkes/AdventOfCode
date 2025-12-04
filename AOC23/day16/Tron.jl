function getarray(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    a = []
    open(filepath) do file
        for ln in eachline(file)
            push!(a,[])
            for char in ln
                push!(a[end],char)
            end
        end
    end
    return mapreduce(permutedims,vcat,a)
end

# function move(lb,grid,Energised)
#     si = size(grid,1)
#     sj = size(grid,2)
#     symb = grid[lb[1],lb[2]]
#     if lb[3] == :right
#         if symb == '.' || symb == '-'
#             @goto right
#         elseif symb == '\\'
#             @goto down
#         elseif symb == '/'
#             @goto up
#         elseif symb == '|'
#             @goto updown
#         end
#     elseif lb[3] == :down
#         if symb == '.' || symb == '|'
#             @goto down
#         elseif symb == '\\'
#             @goto right
#         elseif symb == '/'
#             @goto left
#         elseif symb == '-'
#             @goto leftright
#         end
#     elseif lb[3] == :left
#         if symb == '.' || symb == '-'
#             @goto left
#         elseif symb == '\\'
#             @goto up
#         elseif symb == '/'
#             @goto down
#         elseif symb == '|'
#             @goto updown
#         end
#     elseif lb[3] == :up
#         if symb == '.' || symb == '|'
#             @goto up
#         elseif symb == '\\'
#             @goto left
#         elseif symb == '/'
#             @goto right
#         elseif symb == '-'
#             @goto leftright
#         end
#     end
#     @label right
#         lb[2] != sj && (Energised[lb[1],lb[2]+1] = true; return [(lb[1],lb[2]+1,:right)]) ### right ###
#         return []
#     @label down
#         lb[1] != si && (Energised[lb[1]+1,lb[2]] = true; return [(lb[1]+1,lb[2],:down)]) ### down ###
#         return []
#     @label left
#         lb[2] != 1 && (Energised[lb[1],lb[2]-1] = true; return [(lb[1],lb[2]-1,:left)]) ### left ###
#         return []
#     @label up
#         lb[1] != 1 && (Energised[lb[1]-1,lb[2]] = true; return [(lb[1]-1,lb[2],:up)]) ### up ###
#         return []
#     @label updown
#         lb[1] == 1 && (Energised[lb[1]+1,lb[2]] = true; return [(lb[1]+1,lb[2],:down)]) ### updown ###
#         lb[1] == si && (Energised[lb[1]-1,lb[2]] = true; return [(lb[1]-1,lb[2],:up)])
#         Energised[lb[1]-1,lb[2]] = true
#         Energised[lb[1]+1,lb[2]] = true
#         return [(lb[1]-1,lb[2],:up),(lb[1]+1,lb[2],:down)]
#     @label leftright
#         lb[2] == 1 && (Energised[lb[1],lb[2]+1] = true; return [(lb[1],lb[2]+1,:right)]) ### leftright ###
#         lb[2] == sj && (Energised[lb[1],lb[2]-1] = true; return [(lb[1],lb[2]-1,:left)])
#         Energised[lb[1],lb[2]-1] = true
#         Energised[lb[1],lb[2]+1] = true
#         return [(lb[1],lb[2]-1,:left),(lb[1],lb[2]+1,:right)]
# end

function move(lb,grid,Energised)
    si = size(grid,1)
    sj = size(grid,2)
    symb = grid[lb[1],lb[2]]
    if lb[3] == :right
        if symb == '.' || symb == '-'
            lb[2] != sj && (Energised[lb[1],lb[2]+1] = true; return [(lb[1],lb[2]+1,:right)]) ### right ###
            return []
        elseif symb == '\\'
            lb[1] != si && (Energised[lb[1]+1,lb[2]] = true; return [(lb[1]+1,lb[2],:down)]) ### down ###
            return []
        elseif symb == '/'
            lb[1] != 1 && (Energised[lb[1]-1,lb[2]] = true; return [(lb[1]-1,lb[2],:up)]) ### up ###
            return []
        elseif symb == '|'
            lb[1] == 1 && (Energised[lb[1]+1,lb[2]] = true; return [(lb[1]+1,lb[2],:down)]) ### updown ###
            lb[1] == si && (Energised[lb[1]-1,lb[2]] = true; return [(lb[1]-1,lb[2],:up)])
            Energised[lb[1]-1,lb[2]] = true
            Energised[lb[1]+1,lb[2]] = true
            return [(lb[1]-1,lb[2],:up),(lb[1]+1,lb[2],:down)]
        end
    elseif lb[3] == :down
        if symb == '.' || symb == '|'
            lb[1] != si && (Energised[lb[1]+1,lb[2]] = true; return [(lb[1]+1,lb[2],:down)]) ### down ###
            return []
        elseif symb == '\\'
            lb[2] != sj && (Energised[lb[1],lb[2]+1] = true; return [(lb[1],lb[2]+1,:right)]) ### right ###
            return []
        elseif symb == '/'
            lb[2] != 1 && (Energised[lb[1],lb[2]-1] = true; return [(lb[1],lb[2]-1,:left)]) ### left ###
            return []
        elseif symb == '-'
            lb[2] == 1 && (Energised[lb[1],lb[2]+1] = true; return [(lb[1],lb[2]+1,:right)]) ### leftright ###
            lb[2] == sj && (Energised[lb[1],lb[2]-1] = true; return [(lb[1],lb[2]-1,:left)])
            Energised[lb[1],lb[2]-1] = true
            Energised[lb[1],lb[2]+1] = true
            return [(lb[1],lb[2]-1,:left),(lb[1],lb[2]+1,:right)]
        end
    elseif lb[3] == :left
        if symb == '.' || symb == '-'
            lb[2] != 1 && (Energised[lb[1],lb[2]-1] = true; return [(lb[1],lb[2]-1,:left)]) ### left ###
            return []
        elseif symb == '\\'
            lb[1] != 1 && (Energised[lb[1]-1,lb[2]] = true; return [(lb[1]-1,lb[2],:up)]) ### up ###
            return []
        elseif symb == '/'
            lb[1] != si && (Energised[lb[1]+1,lb[2]] = true; return [(lb[1]+1,lb[2],:down)]) ### down ###
            return []
        elseif symb == '|'
            lb[1] == 1 && (Energised[lb[1]+1,lb[2]] = true; return [(lb[1]+1,lb[2],:down)]) ### updown ###
            lb[1] == si && (Energised[lb[1]-1,lb[2]] = true; return [(lb[1]-1,lb[2],:up)])
            Energised[lb[1]-1,lb[2]] = true
            Energised[lb[1]+1,lb[2]] = true
            return [(lb[1]-1,lb[2],:up),(lb[1]+1,lb[2],:down)]
        end
    elseif lb[3] == :up
        if symb == '.' || symb == '|'
            lb[1] != 1 && (Energised[lb[1]-1,lb[2]] = true; return [(lb[1]-1,lb[2],:up)]) ### up ###
            return []
        elseif symb == '\\'
            lb[2] != 1 && (Energised[lb[1],lb[2]-1] = true; return [(lb[1],lb[2]-1,:left)]) ### left ###
            return []
        elseif symb == '/'
            lb[2] != sj && (Energised[lb[1],lb[2]+1] = true; return [(lb[1],lb[2]+1,:right)]) ### right ###
            return []
        elseif symb == '-'
            lb[2] == 1 && (Energised[lb[1],lb[2]+1] = true; return [(lb[1],lb[2]+1,:right)]) ### leftright ###
            lb[2] == sj && (Energised[lb[1],lb[2]-1] = true; return [(lb[1],lb[2]-1,:left)])
            Energised[lb[1],lb[2]-1] = true
            Energised[lb[1],lb[2]+1] = true
            return [(lb[1],lb[2]-1,:left),(lb[1],lb[2]+1,:right)]
        end
    end
end

function LightSimulate(grid,ip = (1,1,:right))
    todo = [ip]
    visited = []
    Energised = zeros(Bool,size(grid)...)
    Energised[ip[1],ip[2]] = true
    while length(todo) != 0
        current = todo[end]
        current in visited && (pop!(todo); continue)
        push!(visited,current)
        for n in move(current,grid,Energised)
            push!(todo,n)
        end
    end
    return sum(Energised)
end

function EveryLightSimulate(filename)
    grid = getarray(filename)
    si = size(grid,1)
    sj = size(grid,2)
    Energy = []
    for i in 1:si
        println(i," - sides")
        push!(Energy,LightSimulate(grid,(i,1,:right)))
        push!(Energy,LightSimulate(grid,(i,sj,:left)))
    end
    for j in 1:sj
        println(j," - top and bottom")
        push!(Energy,LightSimulate(grid,(1,j,:down)))
        push!(Energy,LightSimulate(grid,(si,j,:up)))
    end
    maxE = maximum(Energy)
    write("/home/laurence/nextcloud/AOC23/day16/$(filename[1:end-4])output.txt","$maxE")
    return maxE
end

# @time LightSimulate(getarray("input.txt"))
@time EveryLightSimulate("input.txt")