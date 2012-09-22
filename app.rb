require 'sinatra'
require './degrib'
require 'json'
require 'serialport'
require 'zmq'

$stdout.sync = true


enable  :sessions, :logging

get '/' do
  erb :index
end

post '/wind' do
  speed = Degrib.wind_speed(params[:position])
  direction = Degrib.wind_direction(params[:position])

  letters='abcdefghijklmnopqrs'
  cmd = "#{letters[direction.round/20]}#{letters[speed.round]}"
  puts "send '#{cmd}' to serial port via zmq for #{speed} knots @ #{direction}\n"

  #zmq
  context = ZMQ::Context.new(1)
  outbound = context.socket(ZMQ::DOWNSTREAM)
  outbound.connect("tcp://127.0.0.1:9000")
  status = outbound.send(cmd)

  content_type :json
  {
    speed: speed,
    direction: direction
  }.to_json
end