File.open('words.txt', 'w') do |f|
  20_000.times do
    data = (Array.new(rand(5..9)) { ('a'..'z').to_a.sample }).join('').to_s
    f.print("#{data} ")
  end
  f.puts
end
