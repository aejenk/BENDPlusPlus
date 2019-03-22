video = ['msvideo1','huffyuv','h264','wmv1','wmv2','zmbv','flv1','mpeg4','hevc','rv10','theora']
audio = ['aac','pcm_alaw','opus','libvorbis','wmav1','wmav2','speex','mp3','mp2','ac3','flac','ra_144']
gltch = [[16,24,32,48,64,64,128,128,256],[64,128,128,256,256,384,384,498,512]]
rand_glc = [gltch[1].sample,gltch[0].sample]
rand_vid = video.sample
rand_aud = audio.sample
puts "Converting to #{rand_vid} and #{rand_aud}"
%x[ rm video.mkv; rm audio.mkv; rm a.mp4]
%x[ ffmpeg -i #{ARGV[0]} -map 0:0 -vcodec #{rand_vid} -strict -2 -bsf noise=#{rand_glc[0]} video.mkv -map 0:1 -acodec #{rand_aud} -strict -2 -bsf noise=#{rand_glc[1]} audio.mkv ] 
puts "Converted. Converting back to reasonable form (with noise)"
%x[ ffmpeg -i video.mkv -i audio.mkv -map 0:0 -map 1:0 -strict -2 -pix_fmt yuv420p a.mp4 ]
puts
puts "video: #{rand_vid} (noise=1/#{rand_glc[0]})"
puts "audio: #{rand_aud} (noise=1/#{rand_glc[1]})"