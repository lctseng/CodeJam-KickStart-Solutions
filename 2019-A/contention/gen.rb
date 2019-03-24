f = File.open('large.1.in.txt', 'w')
n = 1_000_000
q = 300
q.times do
  n1 = rand(n)
  n2 = rand(n1..n)
  f.puts "#{n1} #{n2}"
end

q = 30_0
f = File.open('large.3.in.txt', 'w')
piece = n / q
offset = 0
q.times do |i|
  n1 = offset
  n2 = n1 + piece - rand(0..(piece / 2))
  n2 = n if i == q - 1 || n2 > n
  offset = n2
  f.puts "#{n1} #{n2}"
end

q = 30_0
f = File.open('small.1.in.txt', 'w')
piece = n / q
offset = n / 2 - piece / 2
q.times do |i|
  n1 = offset
  n2 = n1 + piece * (i + 1)
  offset = n1 -= piece / 2
  f.puts "#{n1} #{n2}"
end
