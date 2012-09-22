require 'zmq'
require 'serialport'

$stdout.sync = true

context = ZMQ::Context.new(1)

puts "Opening zmq connection for READ"
inbound = context.socket(ZMQ::UPSTREAM)
inbound.bind("tcp://127.0.0.1:9000")

serial_port='/dev/tty.usbmodem1411'
baud_rate = 9600
data_bits = 8
stop_bits = 1
parity = SerialPort::NONE

#serial port open/closing restarts the arduino
sp = SerialPort.open(serial_port, baud_rate, data_bits, stop_bits, parity)
puts "opened serial port #{sp}"

loop do
  cmd = inbound.recv
  puts "send: #{cmd}"
  sp.write(cmd)
end
