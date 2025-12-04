function applyhash(str)
    S = 0
    for c in str
        S += Int(c)
        S *= 17
        S = mod(S,256)
    end
    return S
end

function asciisum(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    S = 0
    open(filepath) do file
        for str in split(readline(file),",")
            S += applyhash(str)
        end
    end
    return S
end

function focussingpower(Boxes)
    S = 0
    for (i,b) in enumerate(Boxes)
        for (j,l) in enumerate(b)
            S += i*j*l[2]
        end
    end
    return S
end

function LensBoxing(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    Boxes = Array{Array}(undef,256)
    for i in eachindex(Boxes)
        Boxes[i] = []
    end
    open(filepath) do file
        for command in split(readline(file),",")
            if command[end] == '-'
                label = command[1:end-1]
                box = applyhash(label) + 1
                deleteat!(Boxes[box], findall(x->x[1]==label,Boxes[box]))
            else
                label = command[1:end-2]
                box = applyhash(label) + 1
                foclen = parse(Int,command[end])
                nolabelflag = true
                for l in Boxes[box]
                    l[1] == label && (l[2] = foclen; nolabelflag = false)
                end
                nolabelflag && push!(Boxes[box],[label,foclen])
            end
        end
    end
    return focussingpower(Boxes)
end

# @time asciisum("input.txt")
@time LensBoxing("input.txt")

