/* s2html_conv.c - HTML conversion & formatting for s2html (external styles.css) */
#include <stdio.h>
#include <string.h>
#include "s2html_event.h"
#include "s2html_conv.h"

/* HTML-escape helper: prints text to dest_fp while replacing &, <, >, " */
static void fputs_escaped(const char *s, FILE *dest_fp)
{
    const unsigned char *p = (const unsigned char *)s;
    while (*p) {
        switch (*p) {
            case '&': fputs("&amp;", dest_fp); break;
            case '<': fputs("&lt;", dest_fp); break;
            case '>': fputs("&gt;", dest_fp); break;
            case '"': fputs("&quot;", dest_fp); break;
            default: fputc(*p, dest_fp);
        }
        p++;
    }
}

/* start_or_end_conv function definitation
   This version links to an external styles.css in the same directory as the output HTML.
*/
void html_begin(FILE* dest_fp, int type) /* type => not used, but can be used to add differnet HTML tags */
{
	/* Add HTML beginning tags into destination file */
	fprintf(dest_fp, "<!DOCTYPE html>\n");
	fprintf(dest_fp, "<html lang=\"en-US\">\n");
	fprintf(dest_fp, "<head>\n");
	fprintf(dest_fp, "<meta charset=\"UTF-8\">\n");
	fprintf(dest_fp, "<title>%s</title>\n", "s2html");
	/* Link to external stylesheet (styles.css) */
	fprintf(dest_fp, "<link rel=\"stylesheet\" href=\"styles.css\">\n");
	fprintf(dest_fp, "</head>\n");
	/* container */
	fprintf(dest_fp, "<body>\n");
	/* use a wrapper with a preformatted block so spacing is preserved */
	fprintf(dest_fp, "<div class=\"code\"><pre>\n");
}

void html_end(FILE* dest_fp, int type) /* type => not used, but can be used to add differnet HTML tags */
{
	/* Add HTML closing tags into destination file */
	fprintf(dest_fp, "</pre></div>\n");
	fprintf(dest_fp, "</body>\n");
	fprintf(dest_fp, "</html>\n");
}


/* source_to_html function definition
   Converts a parser event into HTML and writes to fp.
   All textual content is HTML-escaped to avoid breaking markup.
*/
void source_to_html(FILE* fp, pevent_t *event)
{
#ifdef DEBUG
	/* For debugging: print event type to stderr */
	/* fprintf(stderr, "EVENT type=%d prop=%d len=%d\n", event->type, event->property, event->length); */
#endif

	switch(event->type)
	{
		case PEVENT_PREPROCESSOR_DIRECTIVE:
			fprintf(fp,"<span class=\"preprocess_dir\">");
			fputs_escaped(event->data, fp);
			fprintf(fp,"</span>");
			break;

		case PEVENT_MULTI_LINE_COMMENT:
		case PEVENT_SINGLE_LINE_COMMENT:
			fprintf(fp,"<span class=\"comment\">");
			fputs_escaped(event->data, fp);
			fprintf(fp,"</span>");
			break;

		case PEVENT_STRING:
			fprintf(fp,"<span class=\"string\">");
			fputs_escaped(event->data, fp);
			fprintf(fp,"</span>");
			break;

		case PEVENT_HEADER_FILE:
			/* write header file differently for STD vs USER */
			if(event->property == USER_HEADER_FILE) {
				/* show as "name" */
				fprintf(fp, "\"<span class=\"header_file\">");
				fputs_escaped(event->data, fp);
				fprintf(fp,"</span>\"");
			} else {
				/* show as &lt;name&gt; */
				fprintf(fp, "&lt;<span class=\"header_file\">");
				fputs_escaped(event->data, fp);
				fprintf(fp,"</span>&gt;");
			}
			break;

		case PEVENT_REGULAR_EXP:
		case PEVENT_EOF :
			/* regular plain text (escape it) */
			fputs_escaped(event->data, fp);
			break;

		case PEVENT_NUMERIC_CONSTANT:
			fprintf(fp,"<span class=\"numeric_constant\">");
			fputs_escaped(event->data, fp);
			fprintf(fp,"</span>");
			break;

		case PEVENT_RESERVE_KEYWORD:
			if(event->property == RES_KEYWORD_DATA)
			{
				fprintf(fp,"<span class=\"reserved_key1\">");
				fputs_escaped(event->data, fp);
				fprintf(fp,"</span>");
			}
			else
			{
				fprintf(fp,"<span class=\"reserved_key2\">");
				fputs_escaped(event->data, fp);
				fprintf(fp,"</span>");
			}
			break;

		case PEVENT_ASCII_CHAR:
			fprintf(fp,"<span class=\"ascii_char\">");
			fputs_escaped(event->data, fp);
			fprintf(fp,"</span>");
			break;

		default :
			/* Unknown event: just escape and print so we don't lose data */
			fputs_escaped(event->data, fp);
			break;
	}
}
