require 'fileutils'

if ARGV[0] == nil
    puts "No filename specified. usage: add_files newFile"
    return
end

new_src_name = ARGV[0] + ".c"
new_obj_name = ARGV[0] + ".o"

#add new file
FileUtils.touch "src/" + new_src_name

#update the makefile
array_of_lines = File.open("Makefile", "r").readlines.map( &:chomp )

array_of_lines.each { |line|
    if line[0..6] == "OBJECTS"
        line << " obj/" << new_obj_name
    end
} 

File.open( "Makefile", "w" ) do |f|
    f.puts array_of_lines
end
