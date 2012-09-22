require 'csv'
require 'date'

class Degrib
  def self.read(metric,point)
    file_path = File.expand_path(File.join('data', "ds.#{metric}.bin"))
    raise "can't file file at #{file_path}" unless File.exists?(file_path)
    rows = CSV.parse(`degrib #{file_path} -P -pnt #{point}`)[1..-1]

    rows.sort_by! { |row| (DateTime.now - DateTime.parse(row[-2])).magnitude }
    rows[0][-1].to_f
  end

  def self.wind_direction(point)
    self.read('wdir', point)
  end

  def self.wind_speed(point)
    self.read('wspd', point)
  end
end