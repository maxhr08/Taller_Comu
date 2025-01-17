function [] = main(input_message)
    %Receive message from interface and convert all characters into an
    %array of ASCII binary codes for further codification.
    
    input_message = char(input_message);
    
    message_array = string_to_ASCII(input_message);
   
    %Open txt file to wriite encoded message to.
    encoded_message_FILE = fopen('mensaje_codificado.txt','w');
    
    %Go through the message_array and encode each character
    %Then cast each encoded message to a char to then save it in a txt
    %file.
    for i=1:size(message_array,1)
        encoded_message = codification(message_array(i,:));
        encoded_message = char(encoded_message+'0');
        fprintf(encoded_message_FILE,'%s\n',encoded_message);
    end
    %fprintf(encoded_message_FILE,'%s','EOF');
    
    fclose(encoded_message_FILE);
    
    %Execute python script to send data to Arduino
    system('py ../../python/TallerToSerial.py')
    
end
    
    
    