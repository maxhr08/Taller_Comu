function [Encoded_Message]=codification(message)
% % Use a [15,11] Hamming code.
% m = 4; n = 2^m-1; k = n-m;
    m = 4;
    [H1,G] = hammgen(m);
    Gt=G';
    
    mt=message';
    Encoded_Message=Gt*mt;
    
    for i=1:length(Encoded_Message)
        if rem(Encoded_Message(i),2)==1
            Encoded_Message(i)=1;
        else
            Encoded_Message(i)=0;
        end
    end
    
    Encoded_Message=Encoded_Message';
end 