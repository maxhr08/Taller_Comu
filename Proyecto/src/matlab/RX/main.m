function y = main()
    
    %Read text file with incomming encoded message
    encoded_message_FILE = fopen('../TX/mensaje_codificado.txt','r'); 
    encoded_message_array = [];
    
    while ~feof(encoded_message_FILE) % feof(fid) is true when the file ends
        encoded_message_array = [encoded_message_array; fgetl(encoded_message_FILE)]; % read one line
    end
    
    fclose(encoded_message_FILE);
    
    %Decode incoming message
    decoded_message_array = Hamm_decode(encoded_message_array);
    
    %Error correction on incoming message
    %
    
    %ASCII_to_message
    message = ASCII_to_message(decoded_message_array);
    
    y = message;
    
end