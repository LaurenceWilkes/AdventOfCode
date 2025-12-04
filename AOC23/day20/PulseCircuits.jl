using DataStructures

function getmodules(filename)
    filepath = joinpath(dirname(@__FILE__), "$(filename).txt")
    Broad, FF, Conj = [],[],[]
    open(filepath) do file
        for ln in eachline(file)
            outs = []
            nameid = match(r"\S+",ln).match
            for st in eachmatch(r"[a-z]+",ln[length(nameid)+5:end])
                push!(outs,st.match)
            end
            if nameid[1] == '%'
                push!(FF,[nameid[2:end],outs])
            elseif nameid[1] == '&'
                push!(Conj,[nameid[2:end],outs])
            else
                Broad = copy(outs)
            end
        end
    end
    return Broad, FF, Conj
end


mutable struct FlipFlop
    state::Bool
    dests::Vector{String}
end

function (f::FlipFlop)(loc,x::Bool)
    if !x
        f.state = !(f.state)
        return f.state
    end
end

mutable struct Conjunction
    state::Dict{String, Bool}
    dests::Vector{String}
end

function (f::Conjunction)(loc,x::Bool)
    f.state[loc] = x
    all(values(f.state)) ? (return false) : (return true)
end

mutable struct Broadcaster
    dests::Vector{String}
end

function construct(Broad, FF, Conj)
    Gates = Dict()
    Inputs = Dict()
    Gates["broadcaster"] = Broadcaster(Broad)
    for o in Broad
        !haskey(Inputs,o) && (Inputs[o] = [])
        push!(Inputs[o],"broadcaster")
    end
    for ff in FF
        Gates[ff[1]] = FlipFlop(false,ff[2])
        for o in ff[2]
            !haskey(Inputs,o) && (Inputs[o] = [])
            push!(Inputs[o],ff[1])
        end
    end
    for conj in Conj
        for o in conj[2]
            !haskey(Inputs,o) && (Inputs[o] = [])
            push!(Inputs[o],conj[1])
        end
    end
    for conj in Conj
        Q = Dict()
        for st in Inputs[conj[1]]
            Q[st] = false
        end
        Gates[conj[1]] = Conjunction(Q,conj[2])
    end
    # println(Inputs["rk"])
    return Gates
end

function simulate(Gates)
    todo = Queue{Any}()
    negpulsecount = 1
    pospulsecount = 0
    for o in Gates["broadcaster"].dests
        enqueue!(todo,("broadcaster",false,o))
        negpulsecount += 1
    end
    # println(todo)
    while !isempty(todo)
        cur = dequeue!(todo)
        op = nothing
        haskey(Gates,cur[3]) && (op = Gates[cur[3]](cur[1],cur[2]))
        if !isnothing(op)
            for d in Gates[cur[3]].dests
                enqueue!(todo,(cur[3],op,d))
                op ? (pospulsecount += 1) : (negpulsecount += 1)
            end
        end
    end
    return negpulsecount, pospulsecount
end

function thousandpushes(filename)
    Broad,FF,Conj = getmodules(filename)
    Gates = construct(Broad,FF,Conj)
    negpulsecount, pospulsecount = 0, 0
    for _ in 1:1000
        n,p = simulate(Gates)
        # println(n," ",p)
        negpulsecount += n
        pospulsecount += p
    end
    return negpulsecount*pospulsecount
end


function pushuntil(On,filename)
    Broad,FF,Conj = getmodules(filename)
    Gates = construct(Broad,FF,Conj)
    pushes = 0
    bflag = false
    while true
        pushes += 1
        todo = Queue{Any}()
        for o in Gates["broadcaster"].dests
            enqueue!(todo,("broadcaster",false,o))
        end
        while !isempty(todo)
            cur = dequeue!(todo)
            cur[3] == On && cur[2] == false && (bflag = true; break)
            op = nothing
            haskey(Gates,cur[3]) && (op = Gates[cur[3]](cur[1],cur[2]))
            if !isnothing(op)
                for d in Gates[cur[3]].dests
                    enqueue!(todo,(cur[3],op,d))
                end
            end
        end
        bflag && break
    end
    return pushes
end

@time pushuntil("qx","input")