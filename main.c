#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLOR_CYAN "\033[38;5;81m"
#define COLOR_DIM "\033[38;5;245m"
#define COLOR_RESET "\033[0m"

const char *ordinal(int n) {
    if (n >= 11 && n <= 13) return "th";
    switch (n % 10) {
        case 1: return "st";
        case 2: return "nd";
        case 3: return "rd";
        default: return "th";
    }
}

typedef struct {
    char date[11];
    char time[6];
    char body[512];
} Entry;

int compare(const void *a, const void *b) {
    Entry *ea = (Entry *)a;
    Entry *eb = (Entry *)b;
    int date = strcmp(ea->date, eb->date);
    if (date != 0) return date;
    return strcmp(ea->time, eb->time);
}

void format_date(const char *iso_date, char *out, size_t out_size) {
    int year, month, day;
    sscanf(iso_date, "%d-%d-%d", &year, &month, &day);

    const char *months[] = {
        "January","February","March","April","May","June",
        "July","August","September","October","November","December"
    };

    static const int t[] = {0,3,2,5,0,3,5,1,4,6,2,4};
    if (month < 3) year--;
    int dow = (year + year/4 - year/100 + year/400 + t[month-1] + day) % 7;

    const char *weekdays[] = {
        "Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"
    };

    snprintf(out, out_size, "%s, %d%s %s", weekdays[dow], day, ordinal(day), months[month-1]);
}

int parse_entry(const char *buffer, Entry *entry) {
    char *pos = strstr(buffer, "\"eventstart\":\"");
    if (!pos) return 0;

    pos += strlen("\"eventstart\":\"");
    int i = 0;
    while (*pos != 'T') entry->date[i++] = *pos++;
    entry->date[i] = '\0';
    pos++;

    i = 0;
    while (*pos != '"') entry->time[i++] = *pos++;
    entry->time[i] = '\0';

    pos = strstr(buffer, "\"body\":\"");
    if (!pos) return 0;

    pos += strlen("\"body\":\"");
    i = 0;
    while (*pos != '"' && i < (int)sizeof(entry->body) - 1)
        entry->body[i++] = *pos++;
    entry->body[i] = '\0';

    return 1;
}

int load_entries(Entry *entries, int max) {
    char buffer[4096];
    int count = 0;

    FILE *fp = popen("remind -t14 --json ~/.reminders", "r");
    if (!fp) return 0;

    while (fgets(buffer, sizeof(buffer), fp) && count < max) {
        if (strstr(buffer, "[") || strstr(buffer, "{\"banner\"") || strstr(buffer, "]"))
            continue;
        if (parse_entry(buffer, &entries[count]))
            count++;
    }

    pclose(fp);
    return count;
}

void print_entries(Entry *entries, int count) {
    char current_date[11] = "";
    char formatted[64];
    if (count == 0) {
        printf("  %sNone%s\n", COLOR_DIM, COLOR_RESET);
    };
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].date, current_date) != 0) {
            strcpy(current_date, entries[i].date);
            format_date(current_date, formatted, sizeof(formatted));
            printf("%s%s%s\n", COLOR_CYAN, formatted, COLOR_RESET);
        }
        printf("  %s%s%s  %s\n", COLOR_DIM, entries[i].time, COLOR_RESET, entries[i].body);
    }
}

int main(void) {
    Entry entries[64];

    printf("\nReminders\n\n");

    int count = load_entries(entries, 64);
    qsort(entries, count, sizeof(Entry), compare);
    print_entries(entries, count);
}