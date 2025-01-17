function message = ASCII_to_message(decoded_char_array)
    
    message = '';
    flag = false;
    already_written = false;
    
    for i=1:size(decoded_char_array,1)
        if flag
            message = strcat(message,{' '},char(ASCII_to_char(decoded_char_array(i,:))));
            flag = false;
            already_written = true;
        end
        %If its a space, then activate flag
        if decoded_char_array(i,:)=='00100000'
            flag = true;
        elseif ~already_written
            message = strcat(message,char(ASCII_to_char(decoded_char_array(i,:))));
        end
        already_written = false;
    end
end