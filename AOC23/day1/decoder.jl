function decoder1(file)
    filepath = joinpath(dirname(@__FILE__), file)

    listofcoords = []
    open(filepath) do file
        for ln in eachline(file)
            f = ln[findfirst(r"\d",ln)]
            rln = reverse(ln)
            l = rln[findfirst(r"\d",rln)]
            push!(listofcoords,parse(Int,f*l))
        end
    end
    println(sum(listofcoords))
end

function firstnumber(ln)
    integerwords = ["one","two","three","four","five","six","seven","eight","nine"]
    lowestind = length(ln)+1:length(ln)+1
    lf = ""
    for (f,wd) in enumerate(integerwords)
        index = findfirst(wd,ln)
        !isnothing(index) && (index < lowestind && (lowestind = index;lf = f))
    end
    digind = findfirst(r"\d",ln)
    !isnothing(digind) && (digind < lowestind && return ln[digind])
    return string(lf)
end

function lastnumber(ln)
    ln = reverse(ln)
    integerwords = ["eno","owt","eerht","ruof","evif","xis","neves","thgie","enin"]
    lowestind = length(ln)+1:length(ln)+1
    lf = ""
    for (f,wd) in enumerate(integerwords)
        index = findfirst(wd,ln)
        !isnothing(index) && (index < lowestind && (lowestind = index;lf = f))
    end
    digind = findfirst(r"\d",ln)
    !isnothing(digind) && (digind < lowestind && return ln[digind])
    return string(lf)
end

function decoder2(file)
    filepath = joinpath(dirname(@__FILE__), file)
    listofcoords = []
    open(filepath) do file
        for ln in eachline(file)
            f = firstnumber(ln)
            l = lastnumber(ln)
            push!(listofcoords,parse(Int,f*l))
        end
    end
    println(sum(listofcoords))
end

# @time decoder1("input.txt")
@time decoder2("input.txt")