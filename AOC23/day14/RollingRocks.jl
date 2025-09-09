using Plots

function getarray(filename)
    filepath = joinpath(dirname(@__FILE__), filename)
    a = []
    open(filepath) do file
        for ln in eachline(file)
            push!(a,[])
            for char in ln
                push!(a[end],char)
            end
        end
    end
    return mapreduce(permutedims,vcat,a)
end

# function getdualarray(filename)
#     filepath = joinpath(dirname(@__FILE__), filename)
#     plat = []
#     platmat = []
#     open(filepath) do file
#         for ln in eachline(file)
#             push!(plat,[])
#             for char in ln
#                 if char == 'O'
#                     push!(platmat,true)

#                 else
#                     push!(platmat,false)
#                 push!(plat[end],char)
#             end
#         end
#     end
#     plat = mapreduce(permutedims,vcat,a)
#     platmat = zeros(Bool,size(plat)...)
#     for (j,col) in eachcol(plat)
#         for i in eachindex(col)
#             col[i] == 'O' && (platmat[i,j] = true)
#         end
#     end
#     return plat, platmat
# end

function rollnorth!(plat)
    for col in eachcol(plat)
        for (i,r) in enumerate(col)
            if r == 'O'
                for j in i-1:-1:1
                    col[j] != '.' && (col[i] = '.'; col[j+1] = 'O'; break)
                    j == 1 && (col[i] = '.'; col[1] = 'O')
                end
            end
        end
    end
end

function calcload(plat)
    load = 0
    sp = size(plat,1)
    for col in eachcol(plat)
        for (i,r) in enumerate(col)
            if r == 'O'
                load += sp-i+1
            end
        end
    end
    return load
end

function spin!(plat)
    si = size(plat,1)
    sj = size(plat,2)
    for j in 1:sj           ## North ##
        for i in 1:si
            if plat[i,j] == 'O'
                for k in i-1:-1:1
                    plat[k,j] != '.' && (plat[i,j] = '.'; plat[k+1,j] = 'O'; break)
                    k == 1 && (plat[i,j] = '.'; plat[1,j] = 'O')
                end
            end
        end
    end
    for i in 1:si           ## West ##
        for j in 1:sj
            if plat[i,j] == 'O'
                for k in j-1:-1:1
                    plat[i,k] != '.' && (plat[i,j] = '.'; plat[i,k+1] = 'O'; break)
                    k == 1 && (plat[i,j] = '.'; plat[i,1] = 'O')
                end
            end
        end
    end
    for j in 1:sj           ## South ##
        for i in si:-1:1
            if plat[i,j] == 'O'
                for k in i+1:si
                    plat[k,j] != '.' && (plat[i,j] = '.'; plat[k-1,j] = 'O'; break)
                    k == si && (plat[i,j] = '.'; plat[si,j] = 'O')
                end
            end
        end
    end
    for i in 1:si           ## East ##
        for j in sj:-1:1
            if plat[i,j] == 'O'
                for k in j+1:sj
                    plat[i,k] != '.' && (plat[i,j] = '.'; plat[i,k-1] = 'O'; break)
                    k == sj && (plat[i,j] = '.'; plat[i,sj] = 'O')
                end
            end
        end
    end
end

function northload(filename)
    plat = getarray(filename)
    rollnorth!(plat)
    calcload(plat)
end

function findspinloop!(plat)
    Hist = [] ### Absolute first port of call if too much memory is used is to simplify this...
    push!(Hist,copy(plat))
    j = 0
    # counter = 3
    while true
        j += 1
        spin!(plat)
        for (i,h) in enumerate(Hist)
            plat == h && (return Hist[i+mod(1000000000-i+1,j-i+1)])
        end
        push!(Hist,copy(plat))
        mod(j,1000) == 0 && println(j," spins")
        # counter -= 1
    end
end

function FindBill(filename)
    plat = getarray(filename)
    hplat = findspinloop!(plat)
    calcload(hplat)
end

# @time northload("input.txt")
@time FindBill("input.txt")

# plat = getarray("eg.txt")
# display(plat)
# spin!(plat)