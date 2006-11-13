/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2004-2006                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

/*  This code generated by:
 *      Author : thomas
 *      Date   : Thu Jun 19 2003
 */



#ifndef CLASSIFIERCODEDOCUMENT_H
#define CLASSIFIERCODEDOCUMENT_H

#include <qmap.h>
#include <qstring.h>
#include <qptrlist.h>

#include "classifier.h"
#include "codeaccessormethod.h"
#include "codedocument.h"
#include "codeoperation.h"
#include "codeclassfield.h"
#include "codeclassfieldlist.h"
#include "umlassociationlist.h"

class UMLRole;

/**
  * class ClassifierCodeDocument
  * A CodeDocument which represents a UMLClassifier (e.g. a Class or Interface) 
  */

class ClassifierCodeDocument : public CodeDocument
{
    friend class HierarchicalCodeBlock;
    Q_OBJECT
public:

    // Constructors/Destructors
    //


    /**
     * Empty Constructor
     */
    ClassifierCodeDocument ( UMLClassifier * parent );

    /**
     * Empty Destructor
     */
    virtual ~ClassifierCodeDocument ( );

    /**
     * Add a CodeClassField object to the m_classfieldVector List
     */
    bool addCodeClassField ( CodeClassField * add_object );

    /**
     * Remove a CodeClassField object from m_classfieldVector List
     */
    bool removeCodeClassField ( CodeClassField * remove_object );

    /**
     * Get the list of CodeClassField objects held by m_classfieldVector
     * @return CodeClassFieldList list of CodeClassField objects held by
     * m_classfieldVector
     */
    CodeClassFieldList * getCodeClassFieldList ( );

    // some Utility methods

    /**
     * Return if the parent classifier is an interface
     */
    bool parentIsInterface();

    /**
     * Return if the parent classifier is a class
     */
    bool parentIsClass();

    /**
     * Tell if one or more codeclassfields are derived from any kind of association.
     */
    bool hasAssociationClassFields();
    /**
     * Tell if one or more codeclassfields are derived from attributes.
     */
    bool hasAttributeClassFields();

    /**
     * Tell if any of the accessor classfields will be of lists of objects.
     */
    bool hasObjectVectorClassFields();

    /**
     * Does this object have any classfields declared?
     */
    bool hasClassFields();

    /**
     * Get a list of codeoperation objects held by this classifiercodedocument.
     * @return      QPtrList<CodeOperation>
     */
    QPtrList<CodeOperation> getCodeOperations ( );

    /** Get a list of codeclassifier objects held by this classifiercodedocument that meet the passed criteria.
     * @return      CodeClassFieldList
     */
    CodeClassFieldList getSpecificClassFields (CodeClassField::ClassFieldType cfType);

    /** Get a list of codeclassifier objects held by this classifiercodedocument that meet the passed criteria.
     * @return      CodeClassFieldList
     */
    CodeClassFieldList getSpecificClassFields (CodeClassField::ClassFieldType cfType, bool isStatic);

    /** Get a list of codeclassifier objects held by this classifiercodedocument that meet the passed criteria.
     * @return      CodeClassFieldList
     */
    CodeClassFieldList getSpecificClassFields (CodeClassField::ClassFieldType cfType, Uml::Visibility visibility);

    /** Get a list of codeclassifier objects held by this classifiercodedocument that meet the passed criteria.
     * @return      CodeClassFieldList
     */
    CodeClassFieldList getSpecificClassFields (CodeClassField::ClassFieldType cfType, bool isStatic, Uml::Visibility visibility);

    /** Using the parent object's UML ID, find the corresponding
     * codeclassfield object in this classifiercodedocument. Returns
     * NULL if no such codeclassfield object exists in this document.
     *
     * @param role_id  0 for role A of the asssociation
     *                 1 for role B of the asssociation
     *                -1 if this is an attribute.
     */
    CodeClassField * findCodeClassFieldFromParentID (Uml::IDType id, int role_id = -1);

    /**
     * Get the value of m_parentclassifier
     * @return the value of m_parentclassifier
     */
    UMLClassifier * getParentClassifier ( );

    // a utility method that allows user to easily add classfield methods to this document
    void addCodeClassFieldMethods(CodeClassFieldList &list );

    /**
    * Utility method to appropriately populate the code classfields for this document.
    */
    virtual void initCodeClassFields ( );

    // cause this classifier code document to synchronize to current policy
    virtual void synchronize();

    /** Will add the code operation in the correct place in the document.
     *  @return bool which is true IF the code operation was added successfully
     */
    virtual bool addCodeOperation (CodeOperation *opBlock) = 0;

    /**
     * Save the XMI representation of this object
     */
    virtual void saveToXMI ( QDomDocument & doc, QDomElement & root );

    /**
     * load params from the appropriate XMI element node.
     */
    virtual void loadFromXMI ( QDomElement & root );

protected:

    /**
     * Load CodeClassFields from XMI element node.
     */
    void loadClassFieldsFromXMI( QDomElement & childElem);

    /** set attributes of the node that represents this class
     * in the XMI document.
     */
    virtual void setAttributesOnNode ( QDomDocument & doc, QDomElement & blockElement);

    /** set the class attributes of this object from
     * the passed element node.
     */
    virtual void setAttributesFromNode ( QDomElement & element);

    // find a specific textblock held by any code class field in this document
    // by its tag
    TextBlock * findCodeClassFieldTextBlockByTag (const QString &tag);

    // add the declaration text blocks for various classfields
    void declareClassFields (CodeClassFieldList & list , CodeGenObjectWithTextBlocks * parent);

    virtual void updateContent( ) = 0;

    // force syncronization of child classfields to their parent objects
    void syncClassFields( );

    // IF the classifier object is modified, this will get called.
    // @fixme We cannot make this virtual because the ClassifierCodeDocument
    //        constructor calls it.
    void syncNamesToParent( );

private:

    CodeClassFieldList m_classfieldVector;
    UMLClassifier* m_parentclassifier;

    // using the passed list, update our inventory of CodeClassFields which are
    // based on UMLRoles (e.g. derived from associations with other classifiers).
    void updateAssociationClassFields ( UMLAssociationList &assocList );

    // update code operations in this document using the parent classifier
    void updateOperations( );

    /**
     * Maps CodeClassFields to UMLObjects. Used to prevent re-adding a class
     * field.
     */
    QMap<UMLObject *,CodeClassField *> m_classFieldMap;

    /**
     * Init from a UMLClassifier object.
     * @param       classifier 
     */
    void init ( UMLClassifier * classifier );

public slots:

    /**
     * Synchronize this document to the attributes/associations of the parent classifier.
     */
    void addAttributeClassField(UMLClassifierListItem *at, bool syncToParentIfAdded = true);
    void addAssociationClassField (UMLAssociation * assoc, bool syncToParentIfAdded = true);
    void removeAttributeClassField(UMLClassifierListItem *at);
    void removeAssociationClassField(UMLAssociation *assoc);
    void addOperation (UMLClassifierListItem * obj);
    void removeOperation (UMLClassifierListItem * obj);
    void syncToParent( );

};

#endif // CLASSIFIERCODEDOCUMENT_H
