function converted_char = char_to_ASCII(char)
    %Convert a single character to ASCII binary code and return value into
    %converted_char.
    
    char=dec2bin(char);
    char=char-'0';

    largo=length(char);

    if largo<=8
        apend=8-largo;
        matrizceros=zeros(1,apend);
        char=[matrizceros char];
    end
    converted_char=[zeros(1,3) char];
end