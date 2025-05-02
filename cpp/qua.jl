using GromovWitten
ve=[(1,1),(1,2),(2,3),(3,1)]
G=FeynmanGraph(ve)
F=FeynmanIntegral(G)
g=[1,0,0]
d=10
weightmax=10
Iq=substitute(feynman_integral_degree_sum(F,d,g))
println(Iq)
Q=quasimodularity_form(Iq,weightmax)
println(Q)
