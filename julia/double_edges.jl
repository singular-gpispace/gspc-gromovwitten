using Nemo


function nv(ve)
    nvb = Set{Int}()
    for (u, v) in ve
        push!(nvb, u)
        push!(nvb, v)
    end
    return length(nvb)
end

function find_equal_pairs(ve)
    equal_pairs = Dict()
    for (i, pair) in enumerate(ve)
        if haskey(equal_pairs, pair)
            push!(equal_pairs[pair], i)
        else
            equal_pairs[pair] = [i]
        end
    end
    indices = [v for v in values(equal_pairs) if length(v) > 1]
    return indices
end
function vector_to_monomial(G, v::Vector{Int64})
    S = polynomial_ring(QQ, :x => 1:nv(G), :q => 1:length(G), :z => 1:nv(G))
    q = S[3]
    v = 2 * v
    vec = []
    for i in eachindex(v)
        push!(vec, q[i]^v[i])
    end
    poly = prod(vec)
    return poly
end
function generate_permutation(l::Vector{Int64}, indices::Vector{Vector{Int64}})
    original_l = copy(l)
    permuted_lists = Set{Vector{Int64}}()  # Use a Set to ensure uniqueness
    group_permutations = []

    for group_indices in indices
        group_elements = l[group_indices]
        push!(group_permutations, collect(permutations(group_elements)))
    end

    # Generate permutations for each group
    for permuted_indices in Iterators.product(group_permutations...)
        temp_l = copy(original_l)
        for (i, group_indices) in enumerate(indices)
            for (j, idx) in enumerate(group_indices)
                temp_l[idx] = permuted_indices[i][j]
            end
        end
        push!(permuted_lists, temp_l)
    end

    return collect(permuted_lists)
end

ve = [(1, 3), (1, 2), (1, 2), (2, 4), (3, 4), (3, 4)]
a = [1, 0, 0, 2, 1, 0]
println(find_equal_pairs(ve))
println("Number of vertices: ", nv(ve))
println(length(ve))
println(vector_to_monomial(ve, a))