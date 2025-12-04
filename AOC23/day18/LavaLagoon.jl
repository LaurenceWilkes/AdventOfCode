function getinstructions(filename)
    Dir, No, Col = [], [], []
    filepath = joinpath(dirname(@__FILE__), filename)
    open(filepath) do file
        for ln in eachline(file)
            push!(Dir,ln[1])
            m = match(r"\d+",ln[3:end]).match
            push!(No,parse(Int,m))
            push!(Col,ln[6+length(m):end-1])
        end
    end
    return Dir, No, Col
end

function hexinstructions(Col)
    Dir, No = [], []
    Q = Dict('0'=>'R','1'=>'D','2'=>'L','3'=>'U')
    for c in Col
        push!(No,parse(Int,c[1:5],base=16))
        push!(Dir,Q[c[6]])
    end
    return Dir, No
end

function symb(A,B)
    (A == B == 'U' || A == B == 'D') && return '┃'
    (A == B == 'L' || A == B == 'R') && return '━'
    A == 'R' && B == 'U' && return '┛'
    A == 'R' && B == 'D' && return '┓'
    A == 'U' && B == 'L' && return '┓'
    A == 'U' && B == 'R' && return '┏'
    A == 'L' && B == 'U' && return '┗'
    A == 'L' && B == 'D' && return '┏'
    A == 'D' && B == 'L' && return '┛'
    A == 'D' && B == 'R' && return '┗'
end

function rimcoordinates(Dir,No)
    ip = CartesianIndex(0,0)
    prevdir = Dir[end]

    Path = Dict(ip=>symb(prevdir,Dir[1]))
    Cornlist = [ip]
    D2C = Dict('U'=>CartesianIndex(1,0),'R'=>CartesianIndex(0,1),'D'=>CartesianIndex(-1,0),'L'=>CartesianIndex(0,-1))
    for i in eachindex(Dir)
        # println(Dir[i])
        Path[ip] = symb(prevdir,Dir[i])
        for j in 1:No[i]-1
            Path[ip + j*D2C[Dir[i]]] = symb(Dir[i],Dir[i])
        end
        # println(symb(Dir[i],Dir[i]))
        ip += No[i] * D2C[Dir[i]]
        prevdir = Dir[i]
        push!(Cornlist,ip)
    end
    i = extrema([c[1] for c in Cornlist])
    r = extrema([c[2] for c in Cornlist])
    return Path, r[1]:r[2], i[1]:i[2]
end

function showmap(map)
    si = size(map,1)
    sj = size(map,2)
    for i in si:-1:1
        for j in 1:sj
            print(map[i,j])
        end
        print("\n")
    end
end

function rimmap(Path,rerange,imrange)
    map = fill('.',length(imrange),length(rerange))
    for k in keys(Path)
        map[k[1]+1-imrange.start,k[2]+1-rerange.start] = Path[k]
    end
    showmap(map)
end

function getinterior(Path, rerange, imrange)
    S = 0
    for x in imrange
        parity = -1
        for y in rerange
            xy = CartesianIndex(x,y)
            if xy in keys(Path)
                if Path[xy] == '┃' || Path[xy] == '┛' || Path[xy] == '┗'
                    parity *= -1
                end
                S += 1
            elseif parity == 1
                S += 1
            end
        end
    end
    return S
end



function rimranges(Dir,No)
    Vertr2c = Dict()
    maxi, mini = 0, 0
    cp = [0,0]
    D2C = Dict('U'=>[1,0],'R'=>[0,1],'D'=>[-1,0],'L'=>[0,-1])
    for i in eachindex(Dir)
        ep = cp .+ D2C[Dir[i]] .* No[i]
        if Dir[i] == 'U'
            !haskey(Vertr2c,cp[1]:ep[1]) && (Vertr2c[cp[1]:ep[1]] = [])
            push!(Vertr2c[cp[1]:ep[1]],cp[2])
        elseif Dir[i] == 'D'
            !haskey(Vertr2c,ep[1]:cp[1]) && (Vertr2c[ep[1]:cp[1]] = [])
            push!(Vertr2c[ep[1]:cp[1]],cp[2])
        elseif Dir[i] == 'L'
            cp[1] > maxi && (maxi = cp[1])
            cp[1] < mini && (mini = cp[1])
        else
            cp[1] > maxi && (maxi = cp[1])
            cp[1] < mini && (mini = cp[1])
        end
        cp = ep
    end
    return Vertr2c, mini:maxi
end

function rowval(i,Vertr2c)
    RV = 0
    J = []
    for r in keys(Vertr2c)
        if i in r
            if i == r.start
                for jr in Vertr2c[r]
                    push!(J,(jr,:bottom))
                end
            elseif i == r.stop
                for jr in Vertr2c[r]
                    push!(J,(jr,:top))
                end
            else
                for jr in Vertr2c[r]
                    push!(J,(jr,:middle))
                end
            end
        end
    end
    sort!(J,by=j->j[1])
    lJ = length(J)
    parity = -1
    tbparity = -1
    for k in 1:lJ-1
        if J[k][2] == :middle
            parity *= -1
            if parity == 1
                RV += J[k+1][1] - J[k][1]
            else
                RV += 1
            end
        elseif J[k][2] == :top
            parity *= -1
            tbparity *= -1
            if parity == 1 || tbparity == 1
                RV += J[k+1][1] - J[k][1]
            else
                RV += 1
            end
        else
            tbparity *= -1
            if parity == 1 || tbparity == 1
                RV += J[k+1][1] - J[k][1]
            else
                RV += 1
            end
        end
    end
    lJ >= 1 && (RV += 1)
    return RV
end

function rangestointerior(Vertr2c,Irange)
    S = 0
    Ipoints = Set()
    for r in keys(Vertr2c)
        push!(Ipoints,r.start)
        push!(Ipoints,r.stop)
    end
    Ipoints = collect(Ipoints)
    sort!(Ipoints)
    lIp = length(Ipoints)
    for n in 1:lIp-1
        S += rowval(Ipoints[n],Vertr2c)
        if Ipoints[n+1] != Ipoints[n]+1
            S += rowval(Ipoints[n]+1,Vertr2c)*(Ipoints[n+1]-Ipoints[n]-1)
        end
    end
    S += rowval(Ipoints[end],Vertr2c)
    return S
end

function part1(filename)
    Dir, No, Col = getinstructions("$(filename).txt")
    Path, rerange, imrange = rimcoordinates(Dir,No)
    # println(rerange," ",imrange)
    # rimmap(Path, rerange, imrange)
    getinterior(Path, rerange, imrange)
end

function part2(filename)
    Dir1, No1, Col = getinstructions("$(filename).txt")
    Dir, No = hexinstructions(Col)
    Vertr2c,Irange = rimranges(Dir,No)
    rangestointerior(Vertr2c,Irange)
end

@time part2("input")