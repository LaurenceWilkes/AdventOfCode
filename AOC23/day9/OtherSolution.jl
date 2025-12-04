using Printf

function IntsFromString(ints::AbstractString)
	map(s -> parse(Int64, s), split(ints, ' '))
end

function ReadHistories(fname::String)
	open(fname, "r") do io
		map(IntsFromString, readlines(io))
	end
end

function Diffs(vector::Vector{Int64})
	map(n -> vector[n+1]-vector[n], 1:length(vector)-1)
end

function AllZero(vector::Vector{Int64})
	all(v -> v == 0, vector)
end

function Extrapolate(history::Vector{Int64}, forward::Bool=true)
	diffs = Diffs(history)
	extrapolated = forward ? last(history) : first(history)
	sgn = forward ? +1 : -1
	if !AllZero(diffs)
		extrapolated += sgn * Extrapolate(diffs, forward)
	end
	extrapolated
end

Extrapolate(forward::Bool) = hist -> Extrapolate(hist, forward)

function PartOne(fname::String)
	"""Sum of extrapolated values for AoC D09 Part One."""
	@printf("PART ONE (%s)\n", fname)
	histories = ReadHistories(fname)
	extrapolations = map(Extrapolate, histories)
	@printf("The sum of extrapolated values is %d.\n", sum(extrapolations))
end

function PartTwo(fname::String)
	"""Sum of extrapolated values for AoC D09 Part Two."""
	@printf("PART TWO (%s)\n", fname)
	histories = ReadHistories(fname)
	extrapolations = map(Extrapolate(false), histories)
	@printf("The sum of extrapolated values is %d.\n", sum(extrapolations))
end

function Run(fname::String)
	@time PartOne(fname)
	@time PartTwo(fname)
end

Run("/home/laurence/nextcloud/AOC23/day9/input.txt")