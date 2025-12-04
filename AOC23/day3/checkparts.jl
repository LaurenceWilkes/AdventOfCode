indices(m) = m.offset, m.offset+lastindex(m.match)-1

function checksurround(m,CurLines,linelength)
    fInd, lInd = indices(m)
    for j in max(fInd-1,1):min(lInd+1,linelength), i in eachindex(CurLines)
        CurLines[i][j] != '.' && !isdigit(CurLines[i][j]) && return true
    end
    return false
end

function checkparts(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    open(filepath) do file
        PartNumbers = []
        CurLines = []
        push!(CurLines,readline(file))
        push!(CurLines,readline(file))
        linelength = length(CurLines[1])
        for m in eachmatch(r"\d+",CurLines[1])
            checksurround(m,CurLines,linelength) && push!(PartNumbers,parse(Int,m.match))
        end
        while !eof(file)
            push!(CurLines,readline(file))
            for m in eachmatch(r"\d+",CurLines[2])
                checksurround(m,CurLines,linelength) && push!(PartNumbers,parse(Int,m.match))
            end
            popfirst!(CurLines)
        end
        for m in eachmatch(r"\d+",CurLines[2])
            checksurround(m,CurLines,linelength) && push!(PartNumbers,parse(Int,m.match))
        end
        println(sum(PartNumbers))
    end
end

function gearpair(astm,CurMatches)
    astnumbers = []
    for em in CurMatches
        for m in em
            fInd, lInd = indices(m)
            (astm.offset-1 in fInd:lInd || astm.offset in fInd:lInd || astm.offset+1 in fInd:lInd) && push!(astnumbers,m.match)
        end
    end
    (length(astnumbers) == 2) && return parse.(Int,astnumbers)
end

function checkgears(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    open(filepath) do file
        GearPairs = []
        CurLines = []
        CurMatches = []
        push!(CurLines,readline(file))
        push!(CurMatches,eachmatch(r"\d+",CurLines[1]))
        push!(CurLines,readline(file))
        push!(CurMatches,eachmatch(r"\d+",CurLines[2]))
        for astm in eachmatch(r"\*",CurLines[1])
            gp = gearpair(astm,CurMatches)
            !isnothing(gp) && push!(GearPairs,gp)
        end
        while !eof(file)
            push!(CurLines,readline(file))
            push!(CurMatches,eachmatch(r"\d+",CurLines[3]))
            for astm in eachmatch(r"\*",CurLines[2])
                gp = gearpair(astm,CurMatches)
                !isnothing(gp) && push!(GearPairs,gp)
            end
            popfirst!(CurLines)
            popfirst!(CurMatches)
        end
        for astm in eachmatch(r"\*",CurLines[2])
            gp = gearpair(astm,CurMatches)
            !isnothing(gp) && push!(GearPairs,gp)
        end
        println(sum(prod.(GearPairs)))
    end
end

@time checkparts("input.txt")
@time checkgears("input.txt")