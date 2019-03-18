#!/usr/bin/env ruby
STDOUT.sync = true

test_case = gets.to_i
test_case.times do
  a, b = gets.split(' ').map(&:to_i)
  n = gets.to_i
  n.times do
    # guess mid
    mid = ((a + b) / 2.0).ceil
    puts mid
    case gets.strip
    when 'CORRECT'
      break
    when 'TOO_BIG'
      b = mid - 1
    when 'TOO_SMALL'
      a = mid
    when 'WRONG_ANSWER'
      STDERR.puts 'Wrong answer!'
      exit
    end
  end
end
