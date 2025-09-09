function readinput(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    WFS = Dict()
    parts = []
    comptoint = Dict("x"=>1,"m"=>2,"a"=>3,"s"=>4)
    open(filepath) do file
        ln = readline(file)
        while !isempty(ln)
            flow = match(r"[a-z]+",ln).match
            lf = length(flow)
            Fl = []
            for fl in split(ln[lf+2:end-1],',')
                if (length(fl) > 1) && (fl[2] in ['>','<'])
                    comp,iq,bound,dest = match(r"(?<comp>[xmas])(?<iq>[><])(?<bound>\d+):(?<dest>[a-zA-Z]+)",fl)
                    push!(Fl,[comptoint[comp],iq,parse(Int,bound),dest])
                else
                    push!(Fl,fl)
                end
            end
            WFS[flow] = Fl
            ln = readline(file)
        end
        while !eof(file)
            ln = readline(file)
            p = []
            for m in eachmatch(r"\d+",ln)
                push!(p,parse(Int,m.match))
            end
            push!(parts,Dict("x"=>p[1],"m"=>p[2],"a"=>p[3],"s"=>p[4]))
        end
    end
    return WFS, parts
end

# function runworkflow(part,flow,WFS)
#     flow == "A" && return true
#     flow == "R" && return false
#     for fl in WFS[flow]
#         if isa(fl,AbstractString)
#             return runworkflow(part,fl,WFS)
#         elseif fl[2] == "<"
#             if part[fl[1]] < fl[3]
#                 return runworkflow(part,fl[4],WFS)
#             end
#         elseif fl[2] == ">"
#             if part[fl[1]] > fl[3]
#                 return runworkflow(part,fl[4],WFS)
#             end
#         end
#     end
# end

# function rating(part)
#     sum(values(part))
# end

# function sumratings(filename)
#     WFS, Parts = readinput(filename)
#     # for part in Parts
#     #     println(Parts)
#     # end
#     S = 0
#     for part in Parts
#         runworkflow(part,"in",WFS) && (S += rating(part))
#     end
#     return S
# end

function countbunch(a)
    S = [0,0,0,0]
    for k in 1:4000
        contflag = [false,false,false,false]
        for (r,i) in a
            !(k in r) && (contflag[i] = true)
        end
        !contflag[1] && (S[1] += 1)
        !contflag[2] && (S[2] += 1)
        !contflag[3] && (S[3] += 1)
        !contflag[4] && (S[4] += 1)
    end
    return prod(S)
end


function flowstep(cb,B,fl,WFS)
    fl == "A" && push!(B,cb)
    fl == "R" && return false
    if isa(fl,AbstractString)
        return runworkflow(part,fl,WFS)
    elseif fl[2] == "<"
        if part[fl[1]] < fl[3]
            1:fl[3]
            return rangebunch(cb,fl[4],WFS)
        end
        1:fl[3]
        return rangebunch(cb,fl[4],WFS)
    elseif fl[2] == ">"
        if part[fl[1]] > fl[3]
            return runworkflow(part,fl[4],WFS)
        end
    end
end

function rangebunch!(B,cb,flow,WFS)
    if flow == "A"
        push!(B,cb)
    elseif flow != "R"
        for fl in WFS[flow]
            if isa(fl,AbstractString)
                return rangebunch!(B,cb,fl,WFS)
            elseif fl[2] == "<"
                rangebunch!(B,[cb...,(1:fl[3]-1,fl[1])],fl[4],WFS)
                push!(cb,(fl[3]:4000,fl[1]))
            elseif fl[2] == ">"
                rangebunch!(B,[cb...,(fl[3]+1:4000,fl[1])],fl[4],WFS)
                push!(cb,(1:fl[3],fl[1]))
            end
        end
    end
end

function combocounter(filename)
    WFS,Parts = readinput(filename)
    B = []
    rangebunch!(B,[],"in",WFS)
    S = 0
    for b in B
        S += countbunch(b)
    end
    return S
end

@time combocounter("input.txt")