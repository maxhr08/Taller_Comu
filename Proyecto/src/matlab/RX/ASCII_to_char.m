function character = ASCII_to_char(decoded_char)
   character = char(bin2dec(reshape(decoded_char,8,[]).'));
end