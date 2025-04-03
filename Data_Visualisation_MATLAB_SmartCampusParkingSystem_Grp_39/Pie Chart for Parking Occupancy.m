% Pie Chart for Parking Occupancy

readChannelID = 2898802;
readAPIKey = '0977080DQJSIGDX2';

[data, time] = thingSpeakRead(readChannelID, 'Fields', [1,2,3,4], 'NumPoints', 1, 'ReadKey', readAPIKey);

occupied = sum(data);  % Count occupied spaces
empty = 4 - occupied;  % Remaining empty spaces

figure;
pie([occupied, empty], {'Occupied', 'Empty'});
title('Parking Occupancy Status');
