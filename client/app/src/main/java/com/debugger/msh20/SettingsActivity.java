package com.debugger.msh20;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;

import androidx.appcompat.app.AppCompatActivity;

public class SettingsActivity extends AppCompatActivity {

  private SharedPreferences pref;
  Button saveIpButton;
  EditText newIP;

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
        WindowManager.LayoutParams.FLAG_FULLSCREEN);
    setContentView(R.layout.activity_settings);
    pref = getSharedPreferences("IP_Pref", MODE_PRIVATE);
    saveIpButton = (Button) findViewById(R.id.bSaveIP);
    newIP = (EditText) findViewById(R.id.edIP);
    onClickSaveIp();
    getIP();
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
}
