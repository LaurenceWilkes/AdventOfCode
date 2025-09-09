function gamelist(ln)
    gamenumber = parse(Int,match(r"^\d+",SubString(ln,6:length(ln))).match)
    RGB = []
    for m in eachmatch(r"[:;][^;]+(?=;|$)",ln)
        r,g,b = 0,0,0
        rs = match(r"\d+ r",m.match)
        !isnothing(rs) && (r = parse(Int,rs.match[1:end-2]))
        gs = match(r"\d+ g",m.match)
        !isnothing(gs) && (g = parse(Int,gs.match[1:end-2]))
        bs = match(r"\d+ b",m.match)
        !isnothing(bs) && (b = parse(Int,bs.match[1:end-2]))
        push!(RGB,(r,g,b))
    end
    return (gamenumber,RGB)
end

function gamepossible(ln,rb,gb,bb)
    (gamenumber,RGB) = gamelist(ln)
    for (r,g,b) in RGB
        (r > rb || g > gb || b > bb) && return (gamenumber,false)
    end
    return (gamenumber,true)
end

function countgoodgames(file)
    rb = 12
    gb = 13
    bb = 14
    filepath = joinpath(dirname(@__FILE__), file)
    open(filepath) do file
        PossibleIDs = []
        for ln in eachline(file)
            gp = gamepossible(ln,rb,gb,bb)
            gp[2] && push!(PossibleIDs,gp[1])
        end
        println(sum(PossibleIDs))
    end
end

function gamepower(ln)
    gamenumber,RGB = gamelist(ln)
    mr,mg,mb = 0,0,0
    for (r,g,b) in RGB
        r > mr && (mr = r)
        g > mg && (mg = g)
        b > mb && (mb = b)
    end
    return mr*mg*mb
end

function mingames(file)
    filepath = joinpath(dirname(@__FILE__), file)
    open(filepath) do file
        powers = []
        for ln in eachline(file)
            gamenumber,RGB = gamelist(ln)
            push!(powers,gamepower(ln))
        end
        println(sum(powers))
    end
end

@time countgoodgames("input.txt")
@time mingames("input.txt")
