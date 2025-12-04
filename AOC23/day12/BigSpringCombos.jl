# function combocount(DB,diagram,pattern,i,ip,hblock=0)
#     hashblock = hblock
#     cpattern = copy(pattern)
#     key = (i,ip,hb)
#     # println(i)
#     if haskey(DB,key)
#         return DB[key]
#     end
#     if c == '#'
#         hashblock += 1
#     elseif c == '?'
#         if hashblock != 0
#             if cpattern == [] || cpattern[1] != hashblock
#                 return combocount(diagram[i+1:end],cpattern,hashblock+1)
#             else
#                 return combocount(diagram[i+1:end],cpattern,hashblock+1)+combocount(diagram[i+1:end],cpattern[2:end],0)
#             end
#         end
#         return combocount(diagram[i+1:end],cpattern,hashblock+1)+combocount(diagram[i+1:end],cpattern,hashblock)
#     elseif c == '.' && hashblock != 0
#         if cpattern == [] || cpattern[1] != hashblock
#             return 0
#         else
#             hashblock = 0
#             popfirst!(cpattern)
#         end
#     end
#     (cpattern == [] && hashblock == 0) && return 1
#     length(cpattern) == 1 && (cpattern[1] == hashblock) && return 1
#     return 0
# end

function combocount(DB,diagram,pattern,i,ip,hb)
    key = (i,ip,hb)
    haskey(DB,key) && return DB[key]

    if i > length(diagram)
        (ip > length(pattern) && hb == 0) && return 1
        (ip == length(pattern) && pattern[ip] == hb) && return 1
        return 0
    end

    if diagram[i] == '#'
        return get!(DB,(i+1,ip,hb+1),combocount(DB,diagram,pattern,i+1,ip,hb+1))
    elseif diagram[i] == '?'
        if hb > 0
            if ip > length(pattern) || pattern[ip] != hb
                return get!(DB,(i+1,ip,hb+1),combocount(DB,diagram,pattern,i+1,ip,hb+1))
            else
                return get!(DB,(i+1,ip,hb+1),combocount(DB,diagram,pattern,i+1,ip,hb+1))+get!(DB,(i+1,ip+1,0),combocount(DB,diagram,pattern,i+1,ip+1,0))
            end
        else
            return get!(DB,(i+1,ip,hb+1),combocount(DB,diagram,pattern,i+1,ip,hb+1))+get!(DB,(i+1,ip,hb),combocount(DB,diagram,pattern,i+1,ip,hb))
        end
    elseif diagram[i] == '.' && hb > 0
        if ip > length(pattern) || pattern[ip] != hb
            return 0
        else
            return get!(DB,(i+1,ip+1,0),combocount(DB,diagram,pattern,i+1,ip+1,0))
        end
    else
        return get!(DB,(i+1,ip,hb),combocount(DB,diagram,pattern,i+1,ip,hb))
    end
end

function sumspringcombos(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    rowcombos = []
    DB = Dict()
    open(filepath) do file
        # counter = 1
        for ln in eachline(file)
            # mod(counter,10) == 0 && println(counter)
            # counter += 1
            diagram = match(r"[.#?]+",ln).match
            diagram = ((diagram*"?")^4)*diagram
            pattern = repeat([parse(Int,m.match) for m in eachmatch(r"\d+",ln)],5)
            push!(rowcombos,combocount(DB,diagram,pattern,1,1,0))
            empty!(DB)
        end
    end
    println(sum(rowcombos))
end

# DB = Dict()
# @time println(combocount(DB,("?###????????"*"?")^4*"?###????????", repeat([3,2,1],5),1,1,0))
# empty!(DB);

@time sumspringcombos("input.txt")
