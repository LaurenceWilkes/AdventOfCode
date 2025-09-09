function readmap(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    map = []
    open(filepath) do file
        for ln in eachline(file)
            push!(map,ln)
        end
    end
    return map
end

function Sloc(map)
    for (i,ln) in enumerate(map)
        for (j,c) in enumerate(ln)
            c == 'S' && return (i,j)
        end
    end
end

function pipeloop(filename)
    map = readmap(filename)
    iS,jS = Sloc(map)
    i,j = iS,jS
    # inchargroups = Dict(:up=>['F','7','|'],:left=>['F','-','L'],:down=>['J','|','L'],:right=>['7','-','J'])
    # outchargroups = Dict(:up=>['F','7','|'],:left=>['F','-','L'],:down=>['J','|','L'],:right=>['7','-','J'])
    char = 'L'
    cdir = :right
    clen = 0
    while char != 'S'
        if cdir == :up
            i -= 1
            char = map[i][j]
            if char == 'F'
                cdir = :right
            elseif char == '7'
                cdir = :left
            elseif char == '|'
                cdir = :up
            end
        elseif cdir == :left
            j -= 1
            char = map[i][j]
            if char == 'F'
                cdir = :down
            elseif char == '-'
                cdir = :left
            elseif char == 'L'
                cdir = :up
            end
        elseif cdir == :down
            i += 1
            char = map[i][j]
            if char == 'J'
                cdir = :left
            elseif char == '|'
                cdir = :down
            elseif char == 'L'
                cdir = :right
            end
        elseif cdir == :right
            j += 1
            char = map[i][j]
            if char == 'J'
                cdir = :up
            elseif char == '-'
                cdir = :right
            elseif char == '7'
                cdir = :down
            end
        end
        clen += 1
    end
    println(clen÷2)
end

function produceloop(iS,jS,map)
    i,j = iS,jS
    # inchargroups = Dict(:up=>['F','7','|'],:left=>['F','-','L'],:down=>['J','|','L'],:right=>['7','-','J'])
    # outchargroups = Dict(:up=>['F','7','|'],:left=>['F','-','L'],:down=>['J','|','L'],:right=>['7','-','J'])
    char = 'L'
    cdir = :right
    # clen = 0
    loop = []
    while char != 'S'
        if cdir == :up
            i -= 1
            char = map[i][j]
            if char == 'F'
                cdir = :right
            elseif char == '7'
                cdir = :left
            elseif char == '|'
                cdir = :up
            end
        elseif cdir == :left
            j -= 1
            char = map[i][j]
            if char == 'F'
                cdir = :down
            elseif char == '-'
                cdir = :left
            elseif char == 'L'
                cdir = :up
            end
        elseif cdir == :down
            i += 1
            char = map[i][j]
            if char == 'J'
                cdir = :left
            elseif char == '|'
                cdir = :down
            elseif char == 'L'
                cdir = :right
            end
        elseif cdir == :right
            j += 1
            char = map[i][j]
            if char == 'J'
                cdir = :up
            elseif char == '-'
                cdir = :right
            elseif char == '7'
                cdir = :down
            end
        end
        # clen += 1
        push!(loop,(i,j))
    end
    # println(clen÷2)
    return loop
end

function isinside(iloc,jloc,map,loop)
    parity = 1
    while iloc != 0
        iloc -= 1
        if (iloc,jloc) in loop
            pipechar = map[iloc][jloc]
            pipechar in ['-','7','J'] && (parity *= -1)
        end
    end
    parity == -1 && return true
    return false
end

function innerpoints(filename)
    map = readmap(filename)
    iS,jS = Sloc(map)
    loop = produceloop(iS,jS,map)
    leni = length(map)
    lenj = length(map[1])
    ptsinside = []
    # println(leni*lenj)
    for i in 1:leni
        parity = 1
        for j in 1:lenj
            if (i,j) in loop
                pipechar = map[i][j]
                (pipechar == '|' || pipechar == '7' || pipechar == 'F') && (parity *= -1)
            else
                parity == -1 && push!(ptsinside,(i,j))
            end
        end
        # println("$(i)th row done")
    end
    println(length(ptsinside))
end

# @time pipeloop("input.txt")
@time innerpoints("input.txt")