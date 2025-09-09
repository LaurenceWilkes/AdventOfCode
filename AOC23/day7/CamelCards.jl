fiveofkind(h) = (h[1] == h[2] == h[3] == h[4] == h[5]) ? true : false

function fourofkind(h)
    (h[1] == h[2] == h[3] == h[4]) && return true
    (h[1] == h[2] == h[3] == h[5]) && return true
    (h[1] == h[2] == h[4] == h[5]) && return true
    (h[1] == h[3] == h[4] == h[5]) && return true
    (h[2] == h[3] == h[4] == h[5]) && return true
    return false
end

function fullhouse(h)
    (h[1] == h[2] == h[3]) && (h[4] == h[5]) && return true
    (h[1] == h[2] == h[4]) && (h[3] == h[5]) && return true
    (h[1] == h[4] == h[3]) && (h[2] == h[5]) && return true
    (h[4] == h[2] == h[3]) && (h[1] == h[5]) && return true
    (h[1] == h[2] == h[5]) && (h[3] == h[4]) && return true
    (h[1] == h[5] == h[3]) && (h[2] == h[4]) && return true
    (h[5] == h[2] == h[3]) && (h[1] == h[4]) && return true
    (h[1] == h[5] == h[4]) && (h[2] == h[3]) && return true
    (h[5] == h[2] == h[4]) && (h[1] == h[3]) && return true
    (h[5] == h[4] == h[3]) && (h[1] == h[2]) && return true
    return false
end

function threeofkind(h)
    (h[1] == h[2] == h[3]) && return true
    (h[1] == h[2] == h[4]) && return true
    (h[1] == h[4] == h[3]) && return true
    (h[4] == h[2] == h[3]) && return true
    (h[1] == h[2] == h[5]) && return true
    (h[1] == h[5] == h[3]) && return true
    (h[5] == h[2] == h[3]) && return true
    (h[1] == h[5] == h[4]) && return true
    (h[5] == h[2] == h[4]) && return true
    (h[5] == h[4] == h[3]) && return true
    return false
end

function twopair(h)
    (h[1] == h[2]) && (h[3] == h[4]) && return true
    (h[1] == h[2]) && (h[3] == h[5]) && return true
    (h[1] == h[2]) && (h[4] == h[5]) && return true
    (h[1] == h[3]) && (h[2] == h[4]) && return true
    (h[1] == h[3]) && (h[2] == h[5]) && return true
    (h[1] == h[3]) && (h[4] == h[5]) && return true
    (h[1] == h[4]) && (h[3] == h[5]) && return true
    (h[1] == h[4]) && (h[2] == h[5]) && return true
    (h[1] == h[4]) && (h[2] == h[3]) && return true
    (h[1] == h[5]) && (h[3] == h[4]) && return true
    (h[1] == h[5]) && (h[2] == h[4]) && return true
    (h[1] == h[5]) && (h[2] == h[3]) && return true
    (h[2] == h[3]) && (h[1] == h[4]) && return true
    (h[2] == h[3]) && (h[1] == h[5]) && return true
    (h[2] == h[3]) && (h[4] == h[5]) && return true
    (h[2] == h[4]) && (h[1] == h[3]) && return true
    (h[2] == h[4]) && (h[1] == h[5]) && return true
    (h[2] == h[4]) && (h[3] == h[5]) && return true
    (h[2] == h[5]) && (h[1] == h[3]) && return true
    (h[2] == h[5]) && (h[1] == h[4]) && return true
    (h[2] == h[5]) && (h[3] == h[4]) && return true
    (h[3] == h[4]) && (h[1] == h[2]) && return true
    (h[3] == h[4]) && (h[1] == h[5]) && return true
    (h[3] == h[4]) && (h[2] == h[5]) && return true
    (h[3] == h[5]) && (h[1] == h[2]) && return true
    (h[3] == h[5]) && (h[1] == h[4]) && return true
    (h[3] == h[5]) && (h[2] == h[4]) && return true
    (h[4] == h[5]) && (h[1] == h[2]) && return true
    (h[4] == h[5]) && (h[1] == h[3]) && return true
    (h[4] == h[5]) && (h[2] == h[3]) && return true
    return false
end

function onepair(h)
    (h[4] == h[5]) && return true
    (h[3] == h[5]) && return true
    (h[2] == h[5]) && return true
    (h[1] == h[5]) && return true
    (h[3] == h[4]) && return true
    (h[2] == h[4]) && return true
    (h[1] == h[4]) && return true
    (h[2] == h[3]) && return true
    (h[1] == h[3]) && return true
    (h[1] == h[2]) && return true
    return false
end

function strongerthan(ha,hb)
    d = Dict('A'=>14, 'K'=>13, 'Q'=>12, 'J'=>11, 'T'=>10, '9'=>9, '8'=>8, '7'=>7, '6'=>6, '5'=>5, '4'=>4, '3'=>3, '2'=>2)
    d[ha[1]] > d[hb[1]] && return true
    d[ha[1]] < d[hb[1]] && return false
    d[ha[2]] > d[hb[2]] && return true
    d[ha[2]] < d[hb[2]] && return false
    d[ha[3]] > d[hb[3]] && return true
    d[ha[3]] < d[hb[3]] && return false
    d[ha[4]] > d[hb[4]] && return true
    d[ha[4]] < d[hb[4]] && return false
    d[ha[5]] > d[hb[5]] && return true
    return false
end

function strongerhand(ha,hb)
    fka = fiveofkind(ha)
    fkb = fiveofkind(hb)
    fka && !fkb && return true
    !fka && fkb && return false
    fka && fkb && return strongerthan(ha,hb)
    fka = fourofkind(ha)
    fkb = fourofkind(hb)
    fka && !fkb && return true
    !fka && fkb && return false
    fka && fkb && return strongerthan(ha,hb)
    fka = fullhouse(ha)
    fkb = fullhouse(hb)
    fka && !fkb && return true
    !fka && fkb && return false
    fka && fkb && return strongerthan(ha,hb)
    fka = threeofkind(ha)
    fkb = threeofkind(hb)
    fka && !fkb && return true
    !fka && fkb && return false
    fka && fkb && return strongerthan(ha,hb)
    fka = twopair(ha)
    fkb = twopair(hb)
    fka && !fkb && return true
    !fka && fkb && return false
    fka && fkb && return strongerthan(ha,hb)
    fka = onepair(ha)
    fkb = onepair(hb)
    fka && !fkb && return true
    !fka && fkb && return false
    return strongerthan(ha,hb)
end

lessthan(hb,ha) = strongerhand(ha[1],hb[1])
lessthanjoker(hb,ha) = strongerhandjoker(ha[1],hb[1])

function cardgame(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    hands = []
    open(filepath) do file
        for ln in eachline(file)
            push!(hands,[ln[1:5], parse(Int,ln[7:end])])
        end
    end
    sort!(hands,lt=lessthan)
    S = 0
    for i in eachindex(hands)
        S += i*hands[i][2]
    end
    println(S)
end

function newrules(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    hands = []
    open(filepath) do file
        for ln in eachline(file)
            push!(hands,[ln[1:5], parse(Int,ln[7:end])])
        end
    end
    sort!(hands,lt=lessthanjoker)
    S = 0
    for i in eachindex(hands)
        S += i*hands[i][2]
    end
    println(S)
end

@time cardgame("input.txt")