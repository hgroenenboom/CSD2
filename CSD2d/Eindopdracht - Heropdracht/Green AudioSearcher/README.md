#Project
Voor het builden van de project moet de Projucer v5.4.3 (of later) van ROLI gedownload worden. Dit is het belangrijkste framework van dit project en hiermee kan je de 'Green AudioSearcher.jucer' openen.

## Builds
in de builds folder bevinden zich de projectfiles die door JUCE (Projucer) zijn gegenereerd  
en in de VisualStudio2017 map zijn een aantal executables te vinden. Maar voor een (windows 64) executable kan je beter kijken  
in de "verslag en projectomschrijving" folder. De LinuxMakefile is in deze commit helaas nog niet getest.  

## JuceLibraryCode
code die gegenereert is door de Projucer. Bepalend voor projectsettings en include directories.  

## Source
In de source code bevindt zich mijn zelfgeschreven code. Alles is zelfgeschreven behalve de 'Main.cpp' en een deel van de 'MainComponent.h' en de 'MainComponent.cpp'.  
Deze files zijn door de Projucer gegenereerd.

## Tests
In de tests folder zijn test(s) te vinden voor de afzonderlijke classes uit de Source
