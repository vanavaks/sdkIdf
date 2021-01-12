/*
 * SPIFFS_app.c
 *
 *  Created on: 9 џэт. 2021 у.
 *      Author: Vanavaks
 */

#include "main.h"
#include "esp_spiffs.h"

static const char *TAGSPIFSS = "SPIFFS";

void SPIFFS_makePoints(){
	ESP_LOGI(TAGSPIFSS, "Initializing SPIFFS");

	    esp_vfs_spiffs_conf_t conf = {
	      .base_path = "/spiffs",
	      .partition_label = NULL,
	      .max_files = 5,
	      .format_if_mount_failed = true
	    };

	    // Use settings defined above to initialize and mount SPIFFS filesystem.
	    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
	    esp_err_t ret = esp_vfs_spiffs_register(&conf);

	    if (ret != ESP_OK) {
	        if (ret == ESP_FAIL) {
	            ESP_LOGE(TAGSPIFSS, "Failed to mount or format filesystem");
	        } else if (ret == ESP_ERR_NOT_FOUND) {
	            ESP_LOGE(TAGSPIFSS, "Failed to find SPIFFS partition");
	        } else {
	            ESP_LOGE(TAGSPIFSS, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
	        }
	        return;
	    }

	    size_t total = 0, used = 0;
	    ret = esp_spiffs_info(NULL, &total, &used);
	    if (ret != ESP_OK) {
	        ESP_LOGE(TAGSPIFSS, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
	    } else {
	        ESP_LOGI(TAGSPIFSS, "Partition size: total: %d, used: %d", total, used);
	    }

	    // Use POSIX and C standard library functions to work with files.
	    // First create a file.
	    ESP_LOGI(TAGSPIFSS, "Opening file");
	    FILE* f = fopen("/spiffs/hello.txt", "w");
	    if (f == NULL) {
	        ESP_LOGE(TAGSPIFSS, "Failed to open file for writing");
	        return;
	    }
	    fprintf(f, "Hello World!\n");
	    fclose(f);
	    ESP_LOGI(TAGSPIFSS, "File written");

	    // Check if destination file exists before renaming
	    struct stat st;
	    if (stat("/spiffs/foo.txt", &st) == 0) {
	        // Delete it if it exists
	        unlink("/spiffs/foo.txt");
	    }

	    // Rename original file
	    ESP_LOGI(TAGSPIFSS, "Renaming file");
	    if (rename("/spiffs/hello.txt", "/spiffs/foo.txt") != 0) {
	        ESP_LOGE(TAGSPIFSS, "Rename failed");
	        return;
	    }

	    // Open renamed file for reading
	    ESP_LOGI(TAGSPIFSS, "Reading file");
	    f = fopen("/spiffs/foo.txt", "r");
	    if (f == NULL) {
	        ESP_LOGE(TAGSPIFSS, "Failed to open file for reading");
	        return;
	    }
	    char line[64];
	    fgets(line, sizeof(line), f);
	    fclose(f);
	    // strip newline
	    char* pos = strchr(line, '\n');
	    if (pos) {
	        *pos = '\0';
	    }
	    ESP_LOGI(TAGSPIFSS, "Read from file: '%s'", line);

}

void SPIFFS_unmount(){
	// All done, unmount partition and disable SPIFFS
	    esp_vfs_spiffs_unregister(NULL);
	    ESP_LOGI(TAGSPIFSS, "SPIFFS unmounted");
}
