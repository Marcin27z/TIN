package com.example.tin;

import com.example.tin.dto.AllTutorsRequest;
import com.example.tin.dto.AllTutorsResponse;
import com.example.tin.dto.NewConsultationRequest;
import com.example.tin.entity.Participant;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.embed.swt.FXCanvas;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.stage.Stage;
import javafx.util.converter.DateTimeStringConverter;
import org.json.JSONException;

import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;

public class NewConsultationWindow {

    @FXML
    DatePicker dateBox;
    @FXML
    TextField startTimeBox;
    @FXML
    TextField endTimeBox;
    @FXML
    TextField roomBox;
    @FXML
    Button addButton;
    @FXML
    ComboBox tutorPicker;


    private Serializer serializer;


    public void setSerializer(Serializer serializer) {
        startTimeBox.setPromptText("hh:mm");
        endTimeBox.setPromptText("hh:mm");
        this.serializer = serializer;
        try{
            serializer.serializeAndSend(new AllTutorsRequest());
            AllTutorsResponse response = serializer.deserializeAllTutorsResponse();
            ObservableList<Participant> observableList = FXCollections.observableList(response.getTutorsAccounts());
            tutorPicker.setItems(observableList);
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }

    public void addButtonClicked(ActionEvent actionEvent) throws IOException, JSONException {
        long start = dateBox.getValue().toEpochDay()*86400000  + getSecondFromTextBox(startTimeBox);
        long end = dateBox.getValue().toEpochDay()*86400000 + getSecondFromTextBox(endTimeBox);
        System.out.println("" + start + "  " + end);

        int room = Integer.parseInt(roomBox.getText());
        String tutor = ((Participant)tutorPicker.getValue()).getLogin();

        NewConsultationRequest consultation = new NewConsultationRequest(start, end, tutor, room);
        serializer.serializeAndSend(consultation);
        if (!serializer.deserialize()){
            showDialog("Dodanie konsultacji nie powiodło się!");
        }
        else{
            ((Stage) addButton.getScene().getWindow()).close();
        }
    }

    private int getSecondFromTextBox(TextField field){
        String time = field.getText();
        try {
            int hours = Integer.parseInt(time.substring(0, 2));
            int minutes = Integer.parseInt(time.substring(3, 5));
            if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
                showDialog("Godzina w niepoprawnym formacie!");
                return -1;
            }
            return hours*3600000 + minutes*60000;
        }
        catch (Exception e){
            showDialog("Godzina w niepoprawnym formacie!");
        }
        return -1;
    }

    private void showDialog(String info){
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle("Błąd");
        alert.setHeaderText(info);
        alert.showAndWait();
    }
}
