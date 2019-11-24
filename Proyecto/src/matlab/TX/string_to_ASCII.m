function [converted_message] = string_to_ASCII(s_message)
    %Take in a string and convert each character into ASCII binary code,
    %then place all ASCII codes into converted_message. Each row is a
    %sepperate character.
    
    converted_message = [];
    for i=1:size(s_message,2)
        converted_message = [converted_message; char_to_ASCII(s_message(i))];
    end
end