package com.debugger.msh20;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;

import androidx.appcompat.app.AppCompatActivity;

import org.jetbrains.annotations.NotNull;

public class SettingsActivity extends AppCompatActivity {

  private SharedPreferences pref;
  Button saveIpButton;
  EditText newIP;
  Button bWhite;
  Button bYellow;
  Button bGreen;
  Button bBlue;
  Button bRed;
  Button bRainbow;


  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
        WindowManager.LayoutParams.FLAG_FULLSCREEN);
    setContentView(R.layout.activity_settings);
    pref = getSharedPreferences("IP_Pref", MODE_PRIVATE);
    saveIpButton = (Button) findViewById(R.id.bSaveIP);
    newIP = (EditText) findViewById(R.id.edIP);
    bWhite = (Button) findViewById(R.id.bWhite);
    bYellow = (Button) findViewById(R.id.bYellow);
    bBlue = (Button) findViewById(R.id.bBlue);
    bGreen = (Button) findViewById(R.id.bGreen);
    bRed = (Button) findViewById(R.id.bRed);
    bRainbow = (Button) findViewById(R.id.bRainbow);
    onClickSaveIp();
    getIP();
    onClickBWhite();
    onClickBRed();
    onClickBBlue();
    onClickBYellow();
    onClickBGreen();
    onClickBRainbow();
  }


  private void getIP(){
    String ip = pref.getString("ip", "");
    if (ip != null){
      if(!ip.isEmpty()){
        newIP.setText(ip);
      }
    }
  }

  private void onClickSaveIp(){
    saveIpButton.setOnClickListener(new View.OnClickListener() {
      @Override
      public void onClick(View v) {
        if(newIP.getText() != null){
          saveIp(newIP.getText().toString());
        }
      }
    });
  }

  private void saveIp(String ip){
    SharedPreferences.Editor editor = pref.edit();
    editor.putString("ip", ip);
    editor.apply();
  }

  private void onClickBWhite(){
    bWhite.setOnClickListener(new View.OnClickListener() {
      @Override
      public void onClick(View v) {
        saveColour("White");
      }
    });
  }

  private void onClickBRed(){
    bRed.setOnClickListener(new View.OnClickListener() {
      @Override
      public void onClick(View v) {
        saveColour("Red");
      }
    });
  }

  private void onClickBBlue(){
    bBlue.setOnClickListener(new View.OnClickListener() {
      @Override
      public void onClick(View v) {
        saveColour("Blue");
      }
    });
  }

  private void onClickBYellow(){
    bYellow.setOnClickListener(new View.OnClickListener() {
      @Override
      public void onClick(View v) {
        saveColour("Yellow");
      }
    });
  }

  private void onClickBGreen(){
    bGreen.setOnClickListener(new View.OnClickListener() {
      @Override
      public void onClick(View v) {
        saveColour("Green");
      }
    });
  }

  private void onClickBRainbow(){
    bRainbow.setOnClickListener(new View.OnClickListener() {
      @Override
      public void onClick(View v) {
        saveColour("Rainbow");
      }
    });
  }

  private void saveColour(String currentColour){
    SharedPreferences.Editor editor = pref.edit();
    editor.putString("colour", currentColour);
    editor.apply();
  }

}
