fiveofkind(charcounts) = length(charcounts) <= 1

fourofkind(charcounts) = (V = values(charcounts); any(V .>= -1+sum(V)))

fullhouse(charcounts) = length(charcounts) <= 2

threeofkind(charcounts) = (V = values(charcounts); any(V .>= -2+sum(V)))

twopair(charcounts) = (V = values(charcounts); sum(V .>= 2) >= 2)

onepair(charcounts) = (V = values(charcounts); any(V .>= -3+sum(V)))

function strongerthan(ha,hb)
    d = Dict('A'=>14, 'K'=>13, 'Q'=>12, 'T'=>10, '9'=>9, '8'=>8, '7'=>7, '6'=>6, '5'=>5, '4'=>4, '3'=>3, '2'=>2, 'J'=>1)
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
    charcountsa = Dict{Char, Int}()
    for char in ha
        char != 'J' && (charcountsa[char] = get(charcountsa, char, 0) + 1)
    end
    charcountsb = Dict{Char, Int}()
    for char in hb
        char != 'J' && (charcountsb[char] = get(charcountsb, char, 0) + 1)
    end
    fka = fiveofkind(charcountsa)
    fkb = fiveofkind(charcountsb)
    fka && !fkb && return true
    !fka && fkb && return false
    fka && fkb && return strongerthan(ha,hb)
    fka = fourofkind(charcountsa)
    fkb = fourofkind(charcountsb)
    fka && !fkb && return true
    !fka && fkb && return false
    fka && fkb && return strongerthan(ha,hb)
    fka = fullhouse(charcountsa)
    fkb = fullhouse(charcountsb)
    fka && !fkb && return true
    !fka && fkb && return false
    fka && fkb && return strongerthan(ha,hb)
    fka = threeofkind(charcountsa)
    fkb = threeofkind(charcountsb)
    fka && !fkb && return true
    !fka && fkb && return false
    fka && fkb && return strongerthan(ha,hb)
    fka = twopair(charcountsa)
    fkb = twopair(charcountsb)
    fka && !fkb && return true
    !fka && fkb && return false
    fka && fkb && return strongerthan(ha,hb)
    fka = onepair(charcountsa)
    fkb = onepair(charcountsb)
    fka && !fkb && return true
    !fka && fkb && return false
    return strongerthan(ha,hb)
end



lessthan(hb,ha) = strongerhand(ha[1],hb[1])

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

@time cardgame("input.txt")