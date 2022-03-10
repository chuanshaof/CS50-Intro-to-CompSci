package edu.harvard.cs50.pokedex;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.net.URL;

public class PokemonActivity extends AppCompatActivity {
    private TextView nameTextView;
    private TextView numberTextView;
    private TextView type1TextView;
    private TextView type2TextView;
    private String url;
    private RequestQueue requestQueue;
    private Button Catch;
    private ImageView spriteImageView;

    private TextView descriptionTextView;

    private String pokename;
    private Boolean caught;

    private String description_url;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pokemon);

        requestQueue = Volley.newRequestQueue(getApplicationContext());
        url = getIntent().getStringExtra("url");
        nameTextView = findViewById(R.id.pokemon_name);
        numberTextView = findViewById(R.id.pokemon_number);
        type1TextView = findViewById(R.id.pokemon_type1);
        type2TextView = findViewById(R.id.pokemon_type2);
        Catch = findViewById(R.id.pokemon_catch);
        spriteImageView = findViewById(R.id.pokemon_sprite);
        descriptionTextView = findViewById(R.id.pokemon_description);

        load();
    }

    public void load() {
        type1TextView.setText("");
        type2TextView.setText("");

        JsonObjectRequest request = new JsonObjectRequest(Request.Method.GET, url, null, new Response.Listener<JSONObject>() {
            @Override
            public void onResponse(JSONObject response) {
                try {
                    pokename = (response.getString("name"));

                    nameTextView.setText(capitalize(pokename));
                    numberTextView.setText(String.format("#%03d", response.getInt("id")));

                    // Getting prefix of "Caught" status
                    caught = getPreferences(Context.MODE_PRIVATE).getBoolean(pokename, false);

                    // If not caught
                    if (caught == false) {
                        Catch.setText("Catch");
                    }
                    // If caught
                    else {
                        Catch.setText("Release");
                    }

                    // Get description url
                    description_url = response.getJSONObject("species").getString("url");
                    Log.d("pokedex","in response: "+ description_url);

                    // Request for description API and get description
                    JsonObjectRequest request_description = new JsonObjectRequest(Request.Method.GET, description_url, null, new Response.Listener<JSONObject>() {
                        @Override
                        public void onResponse(JSONObject response) {
                            try {
                                String description = response.getJSONArray("flavor_text_entries").getJSONObject(0).getString("flavor_text");
                                descriptionTextView.setText(description);
                            } catch (JSONException e) {
                                Log.e("cs50", "Pokemon_description json error", e);
                            }
                        }
                    }, new Response.ErrorListener() {
                        @Override
                        public void onErrorResponse(VolleyError error) {
                            Log.e("cs50", "Pokemon description error", error);
                        }
                    });
                    requestQueue.add(request_description);

                    // Downloading sprite
                    String sprite_url = response.getJSONObject("sprites").getString("front_default");
                    new DownloadSpriteTask().execute(sprite_url);

                    JSONArray typeEntries = response.getJSONArray("types");
                    for (int i = 0; i < typeEntries.length(); i++) {
                        JSONObject typeEntry = typeEntries.getJSONObject(i);
                        int slot = typeEntry.getInt("slot");
                        String type = typeEntry.getJSONObject("type").getString("name");

                        if (slot == 1) {
                            type1TextView.setText(capitalize(type));
                        }
                        else if (slot == 2) {
                            type2TextView.setText(capitalize(type));
                        }
                    }

                    // Get description url
                    final String description_url = response.getJSONObject("species").getString("url");

                } catch (JSONException e) {
                    Log.e("cs50", "Pokemon json error", e);
                }
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                Log.e("cs50", "Pokemon details error", error);
            }
        });

        JsonObjectRequest request_description = new JsonObjectRequest(Request.Method.GET, description_url, null, new Response.Listener<JSONObject>() {
            @Override
            public void onResponse(JSONObject response) {
                try {
                    JSONArray description = response.getJSONArray("flavor_text_entries");
                    JSONObject description2 = description.getJSONObject(0);
                    String description3 = description2.getString("flavor_text");
                    descriptionTextView.setText(description3);
                } catch (JSONException e) {
                    Log.e("cs50", "Pokemon_description json error", e);
                }
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                Log.e("cs50", "Pokemon description error", error);
            }
        });

        requestQueue.add(request);
    }


    private class DownloadSpriteTask extends AsyncTask<String, Void, Bitmap> {
        @Override
        protected Bitmap doInBackground(String... strings) {
            try {
                URL url = new URL(strings[0]);
                return BitmapFactory.decodeStream(url.openStream());
            }
            catch (IOException e) {
                Log.e("cs50", "Download sprite error", e);
                return null;
            }
        }

        @Override
        protected void onPostExecute(Bitmap bitmap) {
            spriteImageView.setImageBitmap(bitmap);
        }
    }

    public void toggleCatch(View view) {
        caught = getPreferences(Context.MODE_PRIVATE).getBoolean(pokename, false);

        // Releasing
        if (caught == true) {
            Catch.setText("Catch");
            getPreferences(Context.MODE_PRIVATE).edit().remove(pokename).commit();
        }
        // Catching
        else {
            Catch.setText("Release");
            getPreferences(Context.MODE_PRIVATE).edit().putBoolean(pokename, true).commit();
        }
    }

    // Capitalize the first letter of a string
    public String capitalize(String string) {
        return (string.substring(0, 1).toUpperCase() + string.substring(1));
    }
}
