require 'sinatra'
require './degrib'
require 'json'
require 'serialport'

SERIAL_PORT='/dev/tty.usbmodem1411'
NDFD_DATA='/Users/kpf/Desktop/'

baud_rate = 9600
data_bits = 8
stop_bits = 1
parity = SerialPort::NONE

#serial port open/closing restarts the arduino
sp = SerialPort.open(SERIAL_PORT, baud_rate, data_bits, stop_bits, parity)
puts "opened serial port #{sp}"

enable  :sessions, :logging

get '/' do
  erb :index
end

post '/wind' do
  speed = Degrib.wind_speed(params[:position])
  direction = Degrib.wind_direction(params[:position])

  letters='abcdefghijklmnopqrs'
  cmd = "#{letters[direction.round/20]}#{letters[speed.round]}"
  puts "will send '#{cmd}' to serial port for #{speed} knots @ #{direction}\n"

  sp.write(cmd)

  content_type :json
  {
    speed: speed,
    direction: direction
  }.to_json
end