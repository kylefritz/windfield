require 'csv'

class Degrib
  def self.read(metric,point)
    file_path = File.expand_path(File.join('data', "ds.#{metric}.bin"))
    raise "can't file file at #{file_path}" unless File.exists?(file_path)
    CSV.parse(`degrib #{file_path} -P -pnt #{point}`)
  end

  def self.wind_direction(point)
    vals = self.read('wdir', point)
    #TODO: take time into account
    vals[1][-1].to_f
  end
  def self.wind_speed(point)
    vals = self.read('wspd', point)
    #TODO: take time into account
    vals[1][-1].to_f
  end
end