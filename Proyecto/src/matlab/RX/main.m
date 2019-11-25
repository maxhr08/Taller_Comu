function [decoded_message, corrected_decoded_message] = main()
    
    %Execute python script to fetch received data from Arduino
    system('py ../../python/TallerFromSerial.py')

    %Read text file with incomming encoded message
    encoded_message_FILE = fopen('../TX/mensaje_codificado.txt','r'); 
    encoded_message_array = [];
    corrected_decoded_message_array = [];
    corrected_coded_message_array = [];
    
    while ~feof(encoded_message_FILE) % feof(fid) is true when the file ends
        encoded_message_array = [encoded_message_array; fgetl(encoded_message_FILE)]; % read one line
    end
    
    fclose(encoded_message_FILE);
    
    %Error correction on incoming message
    for i=1:size(encoded_message_array,1)
        corrected_coded_message_array = [corrected_coded_message_array; error_correction(encoded_message_array(i,:))];
    end
    
    %Decode corrected encoded message
    corrected_coded_message_array = double_array_to_binary(corrected_coded_message_array);
    corrected_decoded_message_array = Hamm_decode(corrected_coded_message_array);
    
    %ASCII_to_corrected_message
    corrected_decoded_message = ASCII_to_message(corrected_decoded_message_array);
    
    %Decode incoming message
    decoded_message_array = Hamm_decode(encoded_message_array);
    
    %ASCII_to_message
    decoded_message = ASCII_to_message(decoded_message_array);
    
end