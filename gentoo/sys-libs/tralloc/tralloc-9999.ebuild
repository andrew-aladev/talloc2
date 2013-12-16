# Copyright 1999-2013 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: /var/cvsroot/gentoo-x86/net-im/vacuum/vacuum-9999.ebuild,v 1.10 2013/09/21 12:55:34 tomwij Exp $

EAPI="5"

inherit cmake-utils git-2

DESCRIPTION="Lightweight implementation of simplified talloc api."
HOMEPAGE="https://github.com/andrew-aladev/tralloc"
EGIT_REPO_URI="git://github.com/andrew-aladev/tralloc.git"

LICENSE="LGPL-3+"
SLOT="0/9999"
KEYWORDS=""

IUSE="debug shared-libs static-libs man test"

IUSE_FEATURES="destructor reference"
for feature in ${IUSE_FEATURES}; do
    IUSE="${IUSE} tralloc_features_${feature}"
done

IUSE_UTILS="buffer dynarr"
for util in ${IUSE_UTILS}; do
    IUSE="${IUSE} tralloc_utils_${util}"
done

RDEPEND="man? ( app-text/asciidoc )"
DEPEND="${RDEPEND}"

src_unpack() {
    git-2_src_unpack
}

src_configure() {
    local mycmakeargs=(
        $(cmake-utils_use debug       TRALLOC_DEBUG)
        $(cmake-utils_use shared-libs TRALLOC_SHARED)
        $(cmake-utils_use static-libs TRALLOC_STATIC)
        $(cmake-utils_use man         TRALLOC_MAN)
        $(cmake-utils_use test        TRALLOC_TEST)
        
        $(cmake-utils_use tralloc2_features_destructor TRALLOC_DESTRUCTOR)
        $(cmake-utils_use tralloc2_features_reference  TRALLOC_REFERENCE)
        
        $(cmake-utils_use tralloc2_utils_buffer TRALLOC_UTILS_BUFFER)
        $(cmake-utils_use tralloc2_utils_dynarr TRALLOC_UTILS_DYNARR)
    )
    cmake-utils_src_configure
}

src_test() {
    cmake-utils_src_test
}

src_install() {
    cmake-utils_src_install
}