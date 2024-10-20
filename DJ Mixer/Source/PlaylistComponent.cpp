/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 11 Feb 2024 12:43:16pm
    Author:  Juan Carlos Henriquez

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include "DJAudioPlayer.h"
#include<fstream>
#include<iostream>


//==============================================================================
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player,
                                     AudioFormatManager &     formatManagerToUse,
                                     AudioThumbnailCache &     cacheToUse, DeckGUI* _deckGUI1):player(_player), deckGUI1(_deckGUI1)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    
    //we define the header (each column text
    tableComponent.getHeader().addColumn("Track title",1,160);
    tableComponent.getHeader().addColumn("Length", 2,160);
    tableComponent.getHeader().addColumn("Artist",3,160);
    tableComponent.getHeader().addColumn("",4,160);
    
    //we define the header color
    tableComponent.getHeader().setColour(TableHeaderComponent::backgroundColourId, juce::Colour(0,128,128));
    tableComponent.getHeader().setColour(TableHeaderComponent::textColourId, juce::Colour(200, 238, 238));
    tableComponent.getHeader().setColour(TableHeaderComponent::outlineColourId, Colours::grey);
    
    //we make visible the table
    addAndMakeVisible(tableComponent);
    
    tableComponent.setModel(this);
    
    
    //to open the CSV to read the names
    juce::File appDir = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
    while (appDir.getFileName() != "FinalProject") {
        appDir = appDir.getParentDirectory();
    }
    
    juce::String relativePath = "names.csv";
    juce::File archivo_file = appDir.getChildFile(relativePath);
    std::string archivoFilePathString = archivo_file.getFullPathName().toStdString();
    const char* archivoFilePath = archivoFilePathString.c_str();

    std::ifstream archivo(archivoFilePath);
        if (!archivo.is_open()) {
            std::cerr << "Error when openning the CSV." << std::endl;
            JUCE_BREAK_IN_DEBUGGER;
        }

        std::string linea;

        //Read the CSV line by line
    while (std::getline(archivo, linea)) {
        std::vector<std::string> fila;
        std::istringstream ss(linea);
        std::string elemento;
        
        while (std::getline(ss, elemento, ';')) {
            size_t pos = elemento.find("\"");
            while (pos != std::string::npos) {
                elemento.erase(pos, 1); // delete "
                pos = elemento.find("\"", pos); // search for the next "
            }
            fila.push_back(elemento);
        }
        
        // add the row to the list
        trackTitles.push_back(fila);
    }
    
    
}

PlaylistComponent::~PlaylistComponent()
{
    
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    //we paint the background
    g.fillAll(juce::Colour(200, 238, 238));
    
    
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colour(0,128,128));
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    std::cout<<getHeight()<<std::endl;

    tableComponent.setBounds(0,0,getWidth(),getHeight());

    //we define the width of each column
    int totalWidth = getWidth();
    int columnWidth = totalWidth / 4;
    tableComponent.getHeader().setColumnWidth(1,columnWidth*2);
    tableComponent.getHeader().setColumnWidth(2,columnWidth/2);
    tableComponent.getHeader().setColumnWidth(3,columnWidth);
    tableComponent.getHeader().setColumnWidth(4,columnWidth/2);
}

int PlaylistComponent::getNumRows ()
{
    return 6; // we have 6 rows
}

void PlaylistComponent::paintRowBackground (Graphics & g,int rowNumber,int width,int height,bool rowIsSelected)
{
    // just highlight selected rows
    if (rowIsSelected)
    {
        g.fillAll(Colours::orange);
    }
    else{
        g.fillAll(juce::Colour(200, 238, 238));
    }
}
void PlaylistComponent::paintCell (Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    g.drawText (trackTitles[rowNumber][columnId-1],
                2, 0,
                width - 4, height,
                Justification::centredLeft,
                true);
    
    
}

Component* PlaylistComponent::refreshComponentForCell (int rowNumber,int columnId,bool isRowSelected,Component *existingComponentToUpdate)
{
    //we define the elements for each column and each row
    
    //we create a button in each row
    if (columnId == 4)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton("play");
            btn->setColour(juce::TextButton::buttonColourId, juce::Colour(0,128,128));
            String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}


void PlaylistComponent::buttonClicked(Button *button)
{
        
        //if the button play is clicked, we load the mp3 file
        int id = std::stoi(button->getComponentID().toStdString());

        if (id==0)
        {
            //for the file path
            juce::File appDir = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
            while (appDir.getFileName() != "FinalProject") {
                appDir = appDir.getParentDirectory();
            }
            
            juce::String relativePath = "Music/Gravity.mp3";
            juce::File audioFile = appDir.getChildFile(relativePath);
            
            //we load the file
            player->loadURL(URL{audioFile});
            // and now the waveformDisplay as well
            deckGUI1->drawWave(URL{audioFile});
            
        }
        
        else if (id==1)
        {
            juce::File appDir = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
            while (appDir.getFileName() != "FinalProject") {
                appDir = appDir.getParentDirectory();
            }
            
            juce::String relativePath = "Music/Pequena_Gran_Revolucion.mp3";
            juce::File audioFile = appDir.getChildFile(relativePath);
            
            player->loadURL(URL{audioFile});
            // and now the waveformDisplay as well
            deckGUI1->drawWave(URL{audioFile});
        }
        
        else if (id==2)
        {
            juce::File appDir = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
            while (appDir.getFileName() != "FinalProject") {
                appDir = appDir.getParentDirectory();
            }
            
            juce::String relativePath = "Music/Que_bonito_es_querer.mp3";
            juce::File audioFile = appDir.getChildFile(relativePath);
            
            player->loadURL(URL{audioFile});
            // and now the waveformDisplay as well
            deckGUI1->drawWave(URL{audioFile});
        }
        else if (id==3)
        {
            juce::File appDir = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
            while (appDir.getFileName() != "FinalProject") {
                appDir = appDir.getParentDirectory();
            }
            
            juce::String relativePath = "Music/Reggae.mp3";
            juce::File audioFile = appDir.getChildFile(relativePath);
            
            player->loadURL(URL{audioFile});
            // and now the waveformDisplay as well
            deckGUI1->drawWave(URL{audioFile});
        }
        else if (id==4)
        {
            juce::File appDir = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
            while (appDir.getFileName() != "FinalProject") {
                appDir = appDir.getParentDirectory();
            }
            
            juce::String relativePath = "Music/SilentCastle.mp3";
            juce::File audioFile = appDir.getChildFile(relativePath);
            
            player->loadURL(URL{audioFile});
            // and now the waveformDisplay as well
            deckGUI1->drawWave(URL{audioFile});
        }
        else if (id==5)
        {
            juce::File appDir = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
            while (appDir.getFileName() != "FinalProject") {
                appDir = appDir.getParentDirectory();
            }
            
            juce::String relativePath = "Music/Viva_la_vida.mp3";
            juce::File audioFile = appDir.getChildFile(relativePath);
            
            player->loadURL(URL{audioFile});
            // and now the waveformDisplay as well
            deckGUI1->drawWave(URL{audioFile});
        }
    
    
}


