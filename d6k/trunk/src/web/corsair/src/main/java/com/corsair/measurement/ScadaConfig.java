package com.corsair.measurement;

/**
 * Created by 洪祥 on 15/8/15.
 */
public class ScadaConfig {
    private String url;
    private Boolean simulate;

    public String getUrl() {
        return url;
    }

    public void setUrl(String url) {
        this.url = url;
    }

    public Boolean getSimulate() {
        return simulate;
    }

    public void setSimulate(Boolean simulate) {
        this.simulate = simulate;
    }
}
