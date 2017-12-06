#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <Eina.h>
#include "eolian_database.h"
#include "eolian_priv.h"

EAPI Eina_Stringshare *
eolian_class_file_get(const Eolian_Class *cl)
{
   return cl ? cl->base.file : NULL;
}

EAPI Eina_Stringshare *
eolian_class_full_name_get(const Eolian_Class *cl)
{
   return cl ? cl->full_name : NULL;
}

EAPI Eina_Stringshare *
eolian_class_name_get(const Eolian_Class *cl)
{
   return cl ? cl->name : NULL;
}

EAPI Eina_Iterator *
eolian_class_namespaces_get(const Eolian_Class *cl)
{
   return ((cl && cl->namespaces) ? eina_list_iterator_new(cl->namespaces)
                                  : NULL);
}

EAPI const Eolian_Class *
eolian_class_get_by_name(const Eolian_Unit *unit, const char *class_name)
{
   if (!unit) return NULL;
   Eina_Stringshare *shr = eina_stringshare_add(class_name);
   Eolian_Class *cl = eina_hash_find(unit->state->unit.classes, shr);
   eina_stringshare_del(shr);
   return cl;
}

EAPI const Eolian_Class *
eolian_class_get_by_file(const Eolian_Unit *unit, const char *file_name)
{
   if (!unit) return NULL;
   Eina_Stringshare *shr = eina_stringshare_add(file_name);
   Eolian_Class *cl = eina_hash_find(unit->state->classes_f, shr);
   eina_stringshare_del(shr);
   return cl;
}

EAPI Eolian_Class_Type
eolian_class_type_get(const Eolian_Class *cl)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(cl, EOLIAN_CLASS_UNKNOWN_TYPE);
   return cl->type;
}

EAPI Eina_Iterator *
eolian_all_classes_get(const Eolian_Unit *unit)
{
   return (unit ? eina_hash_iterator_data_new(unit->state->unit.classes) : NULL);
}

EAPI const Eolian_Documentation *
eolian_class_documentation_get(const Eolian_Class *cl)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(cl, NULL);
   return cl->doc;
}

EAPI Eina_Stringshare*
eolian_class_legacy_prefix_get(const Eolian_Class *cl)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(cl, NULL);
   return cl->legacy_prefix;
}

EAPI Eina_Stringshare*
eolian_class_eo_prefix_get(const Eolian_Class *cl)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(cl, NULL);
   return cl->eo_prefix;
}

EAPI Eina_Stringshare*
eolian_class_event_prefix_get(const Eolian_Class *cl)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(cl, NULL);
   return cl->ev_prefix;
}

EAPI Eina_Stringshare*
eolian_class_data_type_get(const Eolian_Class *cl)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(cl, NULL);
   return cl->data_type;
}

EAPI Eina_Iterator *
eolian_class_inherits_get(const Eolian_Class *cl)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(cl, NULL);
   return (cl->inherits ? eina_list_iterator_new(cl->inherits) : NULL);
}

EAPI Eina_Iterator *
eolian_class_implements_get(const Eolian_Class *cl)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(cl, NULL);
   return (cl->implements ? eina_list_iterator_new(cl->implements) : NULL);
}

EAPI Eina_Iterator *
eolian_class_constructors_get(const Eolian_Class *cl)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(cl, NULL);
   return (cl->constructors ? eina_list_iterator_new(cl->constructors) : NULL);
}

EAPI const Eolian_Function *
eolian_class_function_get_by_name(const Eolian_Class *cl, const char *func_name, Eolian_Function_Type f_type)
{
   Eina_List *itr;
   Eolian_Function *fid;
   if (!cl) return NULL;

   if (f_type == EOLIAN_UNRESOLVED || f_type == EOLIAN_METHOD)
      EINA_LIST_FOREACH(cl->methods, itr, fid)
        {
           if (!strcmp(fid->name, func_name))
              return fid;
        }

   if (f_type == EOLIAN_UNRESOLVED || f_type == EOLIAN_PROPERTY ||
         f_type == EOLIAN_PROP_SET || f_type == EOLIAN_PROP_GET)
     {
        EINA_LIST_FOREACH(cl->properties, itr, fid)
          {
             if (!database_function_is_type(fid, f_type))
               continue;
             if (!strcmp(fid->name, func_name))
                return fid;
          }
     }

   _eolian_log("function '%s' not found in class '%s'", func_name, cl->name);
   return NULL;
}

EAPI Eina_Iterator *
eolian_class_functions_get(const Eolian_Class *cl, Eolian_Function_Type foo_type)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(cl, NULL);
   switch (foo_type)
     {
      case EOLIAN_PROPERTY:
         return (cl->properties ? eina_list_iterator_new(cl->properties) : NULL);
      case EOLIAN_METHOD:
         return (cl->methods ? eina_list_iterator_new(cl->methods) : NULL);
      default: return NULL;
     }
}

EAPI Eina_Iterator *
eolian_class_events_get(const Eolian_Class *cl)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(cl, NULL);
   return (cl->events ? eina_list_iterator_new(cl->events) : NULL);
}

EAPI Eina_Iterator *
eolian_class_parts_get(const Eolian_Class *cl)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(cl, NULL);
   return (cl->parts ? eina_list_iterator_new(cl->parts) : NULL);
}

EAPI Eina_Bool
eolian_class_ctor_enable_get(const Eolian_Class *cl)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(cl, EINA_FALSE);
   return cl->class_ctor_enable;
}

EAPI Eina_Bool
eolian_class_dtor_enable_get(const Eolian_Class *cl)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(cl, EINA_FALSE);
   return cl->class_dtor_enable;
}

EAPI Eina_Stringshare *
eolian_class_c_get_function_name_get(const Eolian_Class *cl)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(cl, NULL);
   Eina_Stringshare *ret;
   Eina_Strbuf *buf = eina_strbuf_new();
   char *bufp;
   eina_strbuf_append(buf, cl->full_name);
   switch (cl->type)
     {
      case EOLIAN_CLASS_INTERFACE:
        eina_strbuf_append(buf, "_interface_get");
        break;
      case EOLIAN_CLASS_MIXIN:
        eina_strbuf_append(buf, "_mixin_get");
        break;
      default:
        eina_strbuf_append(buf, "_class_get");
        break;
     }
   eina_strbuf_replace_all(buf, ".", "_");
   bufp = eina_strbuf_string_steal(buf);
   eina_str_tolower(&bufp);
   ret = eina_stringshare_add(bufp);
   free(bufp);
   eina_strbuf_free(buf);
   return ret;
}

EAPI Eina_Stringshare *
eolian_class_c_name_get(const Eolian_Class *cl)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(cl, NULL);
   Eina_Stringshare *ret;
   Eina_Strbuf *buf = eina_strbuf_new();
   char *bufp;
   eina_strbuf_append(buf, cl->full_name);
   switch (cl->type)
     {
      case EOLIAN_CLASS_INTERFACE:
        eina_strbuf_append(buf, "_INTERFACE");
        break;
      case EOLIAN_CLASS_MIXIN:
        eina_strbuf_append(buf, "_MIXIN");
        break;
      default:
        eina_strbuf_append(buf, "_CLASS");
        break;
     }
   eina_strbuf_replace_all(buf, ".", "_");
   bufp = eina_strbuf_string_steal(buf);
   eina_str_toupper(&bufp);
   ret = eina_stringshare_add(bufp);
   free(bufp);
   eina_strbuf_free(buf);
   return ret;
}

EAPI Eina_Stringshare *
eolian_class_c_data_type_get(const Eolian_Class *cl)
{
   char buf[512];
   EINA_SAFETY_ON_NULL_RETURN_VAL(cl, NULL);
   if (!cl->data_type)
     snprintf(buf, sizeof(buf), "%s_Data", cl->full_name);
   else if (!strcmp(cl->data_type, "null"))
     return eina_stringshare_add("void");
   else
     snprintf(buf, sizeof(buf), "%s", cl->data_type);
   for (char *p = strchr(buf, '.'); p; p = strchr(p, '.'))
     *p = '_';
   return eina_stringshare_add(buf);
}
